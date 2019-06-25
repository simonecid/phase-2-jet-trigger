# Phase 2 jet trigger chain

## Overview

This repo holds tagged releases of the whole chain, together with instructions on how to produce the Vivado IP that can be later put on a board.
Instructions targets a KU115, ```xcku115-flvd1517-2-i``` at 240 MHz by default.
A different device can be targeted by editing ```Phase2JetTrigger_ExportIP.tcl``` in the line ```set_part```. A different clock speed can be set by editing the ```set_period``` line in the same file.

## Building the IP for the whole chain

IP for the whole "histogram-buffer-clustering" chain can be built by running
```bash
vivado_hls -f Phase2JetTrigger_ExportIP.tcl
```
After running that, go preparing a coffee/tea, eating outside, or have a jog... anyway, focus on something else, it may time some time depending on the size of the detector region that is being covered. 
For the barrel it should be quite fast (I think within 5-10 minutes).\
Once it is done, you can get the IPs and copy it to a local folder called ```IPs``` by running the utility script ```GetIPs.sh```:
```bash
source GetIPs.sh
```
Then you are good to go. From then on, you should go in Vivado and plug the stuff together.
