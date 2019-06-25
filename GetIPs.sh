#!/bin/bash

#utility script that takes the newly-produced ips and puts them together in a single folder

mkdir -p IPs

cp HLS_JetClustering/KU115/impl/ip/cern-cms_hls_hls_jet_clustering_*.zip IPs/
cp HLS_Histogrammer/KU115/impl/ip/cern-cms_hls_hls_histogrammer_*.zip IPs/
cp HLS_HistogramBuffer/KU115/impl/ip/cern-cms_hls_hls_histogram_buffer_*.zip IPs/