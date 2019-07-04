#!/bin/bash

#utility script that takes the newly-produced ips and puts them together in a single folder

rm -r IPs/HLS_JetClustering
rm -r IPs/HLS_Histogrammer
rm -r IPs/HLS_HistogramBuffer
rm -r IPs/jet_trigger_chain

mkdir -p IPs/HLS_JetClustering/
mkdir -p IPs/HLS_Histogrammer/
mkdir -p IPs/HLS_HistogramBuffer/

cp -r HLS_JetClustering/KU115/impl/ip/* IPs/HLS_JetClustering/
cp -r HLS_Histogrammer/KU115/impl/ip/* IPs/HLS_Histogrammer/
cp -r HLS_HistogramBuffer/KU115/impl/ip/* IPs/HLS_HistogramBuffer/