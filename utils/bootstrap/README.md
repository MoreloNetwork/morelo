# Bootstrap mirrors

A node whose blockchain is empty downloads it over https from a bootstrap mirror,
then syncs the rest from peers. Mirrors come from `--bootstrap-url` (can be
repeated) or, on mainnet, from the DNSSEC checked TXT records of
`bootstrap.morelonetwork.pl`, `bootstrap.morelo.vip` and `bootstrap.mrl.laj.lol`.
`--no-bootstrap` turns this off.

Mirrors are not trusted. Every block goes through the same checks as a block from
a peer: up to the end of the compiled block hashes every group of 512 blocks must
match them, past it every block is fully verified. A mirror which sends a bad
block, fails or is too slow (under 1 MB/s) is dropped for the next one, keeping
the blocks already added. When none is left the node syncs from peers. An
interrupted bootstrap resumes on the next start (state in
`<data-dir>/bootstrap.state`).

## Running a mirror

Use a synced, non pruned node running the same release. Export a height a day
below the top, rounded down to a multiple of 512 blocks: the file holds nothing
but the blocks, so mirrors exporting at about the same time serve exactly the
same file:

    morelo-blockchain-export --data-dir /var/lib/morelo --block-stop-margin 720 \
      --output-file /var/www/bootstrap/blockchain.raw.tmp

On Linux the export can run while morelod does (not on Windows, where it can crash
while morelod writes). It appends to an existing output file, so starting from a
copy of the file already served only adds the new blocks. A cron line for the
`morelo` user:

    30 4 * * * cd /var/www/bootstrap && cp blockchain.raw blockchain.raw.tmp; morelo-blockchain-export --data-dir /var/lib/morelo --block-stop-margin 720 --output-file blockchain.raw.tmp && mv blockchain.raw.tmp blockchain.raw && sha256sum blockchain.raw > blockchain.raw.sha256

The sha256 file lets operators check their mirrors agree, nodes do not use it.
Appending trusts the blocks already in the file: should a reorganization deeper
than the margin ever replace some of them, delete the file and export it again.

Serve the file itself over https (nodes do not follow redirects) with range
requests, which are on by default for static files in nginx and let nodes resume:

    location = /blockchain.raw {
        root /var/www/bootstrap;
        # nodes stop reading while they verify a batch of blocks
        send_timeout 300s;
    }

## Listing a mirror

Each of the three names holds the same TXT record, space separated https URLs:

    bootstrap.morelonetwork.pl. IN TXT "https://mirror1.example.org/blockchain.raw https://mirror2.example.org/blockchain.raw"

Nodes use the records only when at least two names agree, and try the mirrors in
random order. Without records the feature does nothing.
