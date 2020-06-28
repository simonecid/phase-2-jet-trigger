# Phase 2 jet trigger chain

## Overview

This repo holds tagged releases of the whole chain, together with instructions on how to produce the Vivado IP that can be later put on a board.
Instructions targets a KU115, ```xcku115-flvd1517-2-i``` at 240 MHz by default.\
A different device can be targeted by editing:
 
 * ```Phase2JetTrigger_ExportIP_VU9P.tcl``` in the line ```set_part```. A different clock speed can be set by editing the ```set_period``` line in the same file (I would recommend copying this to a new file if a new device is targeted)

 * ```jet_trigger_chain.tcl```, look for the ```EDIT ME``` bits, there are two in total:
   1) Around line 30, change the ```-part``` argument to target the same part you have set in the HLS
   2) Around line 300, change ```CONFIG.FREQ_HZ``` to have the correct clock speed. **Note: specify the real clock speed. Sometimes we setup the vivado hls script to run at a faster clock speed than we want the thing to run to. Specify the real clock speed here.** For instance, KU115 is prepared at 300 MHz in HLS although we want to run at 240 MHz. Therefore we should put 240000000 here.

## Building the IP for the whole chain

IPs for the whole "histogram-buffer-clustering" chain will be built by running
```bash
vivado_hls -f Phase2JetTrigger_ExportIP_<DEVICE_NAME>.tcl
```
After running that, go preparing a coffee/tea, eating outside, or have a jog... anyway, focus on something else, it may time some time depending on the size of the detector region that is being covered. 
For the barrel it should be quite fast (I think within 5-10 minutes).\
Once it is done, you can get the IPs and copy it to a local folder called ```IPs``` by running the utility script ```GetIPs.sh```:
```bash
source GetIPs.sh
```

## Creating a project with the the three IPs together in a block design

Run
```bash
vivado -mode batch -source jet_trigger_chain.tcl
```
to create a project called jet_trigger_chain with a block design called jet_trigger_chain in which the IPs are linked together.
The script will wrap the block design, sythetise it, and produce an IP in IPs/jet_trigger_chain.

Have fun!