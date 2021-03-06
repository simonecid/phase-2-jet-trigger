
################################################################
# This is a generated script based on design: jet_trigger_chain
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2018.2
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_msg_id "BD_TCL-109" "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source jet_trigger_chain_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./jet_trigger_chain/jet_trigger_chain.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project -force jet_trigger_chain jet_trigger_chain -part xcku115-flvd1517-2-i
}

set_property target_language VHDL [current_project]

# CHANGE DESIGN NAME HERE
variable design_name
set design_name jet_trigger_chain

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_msg_id "BD_TCL-001" "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_msg_id "BD_TCL-002" "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_msg_id "BD_TCL-003" "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_msg_id "BD_TCL-004" "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_msg_id "BD_TCL-005" "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_msg_id "BD_TCL-114" "ERROR" $errMsg}
   return $nRet
}

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################

set_property IP_REPO_PATHS {"IPs/"} [current_fileset]
update_ip_catalog

set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\ 
cern-cms:hls:hls_histogram_buffer:0.1\
cern-cms:hls:hls_histogrammer:0.2\
cern-cms:hls:hls_jet_clustering:0.2\
"

   set list_ips_missing ""
   common::send_msg_id "BD_TCL-006" "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_msg_id "BD_TCL-115" "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

if { $bCheckIPsPassed != 1 } {
  common::send_msg_id "BD_TCL-1003" "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports

  # Create ports
  set barrel_inputs_0_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_0_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} array_type {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} size {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 1} stride {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 30} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}} \
 ] $barrel_inputs_0_0
  set barrel_inputs_10_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_10_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_10 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 10} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_10_0
  set barrel_inputs_11_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_11_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_11 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 11} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_11_0
  set barrel_inputs_12_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_12_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_12 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 12} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_12_0
  set barrel_inputs_13_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_13_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_13 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 13} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_13_0
  set barrel_inputs_14_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_14_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_14 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 14} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_14_0
  set barrel_inputs_15_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_15_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_15 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 15} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_15_0
  set barrel_inputs_16_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_16_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_16 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 16} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_16_0
  set barrel_inputs_17_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_17_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_17 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 17} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_17_0
  set barrel_inputs_18_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_18_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_18 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 18} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_18_0
  set barrel_inputs_19_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_19_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_19 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 19} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_19_0
  set barrel_inputs_1_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_1_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_1 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 1} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_1_0
  set barrel_inputs_20_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_20_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_20 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 20} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_20_0
  set barrel_inputs_21_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_21_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_21 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 21} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_21_0
  set barrel_inputs_22_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_22_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_22 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 22} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_22_0
  set barrel_inputs_23_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_23_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_23 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 23} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_23_0
  set barrel_inputs_2_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_2_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_2 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 2} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_2_0
  set barrel_inputs_3_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_3_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_3 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 3} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_3_0
  set barrel_inputs_4_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_4_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_4 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 4} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_4_0
  set barrel_inputs_5_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_5_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_5 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 5} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_5_0
  set barrel_inputs_6_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_6_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_6 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 6} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_6_0
  set barrel_inputs_7_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_7_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_7 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 7} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_7_0
  set barrel_inputs_8_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_8_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_8 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 8} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_8_0
  set barrel_inputs_9_0 [ create_bd_port -dir I -from 63 -to 0 -type data barrel_inputs_9_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_9 {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value 9} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type automatic dependency {} format long minimum {} maximum {}} value 0} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} struct {field_pt {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value pt} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iPhi {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iPhi} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} field_iEta {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value iEta} enabled {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value true} datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 10} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 20} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}}}}}}}}}} \
 ] $barrel_inputs_9_0
  set d0Valid_0 [ create_bd_port -dir I -type data d0Valid_0 ]
  set_property -dict [ list \
   CONFIG.LAYERED_METADATA {xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value {}} bitwidth {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 1} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0}}}}} \
 ] $d0Valid_0
  set in_clock_240MHz [ create_bd_port -dir I -type clk in_clock_240MHz ]
  set_property -dict [ list \
   CONFIG.FREQ_HZ {240000000} \
 ] $in_clock_240MHz
  set outJets_0_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_0_0 ]
  set outJets_10_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_10_0 ]
  set outJets_11_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_11_0 ]
  set outJets_12_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_12_0 ]
  set outJets_13_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_13_0 ]
  set outJets_14_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_14_0 ]
  set outJets_15_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_15_0 ]
  set outJets_16_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_16_0 ]
  set outJets_17_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_17_0 ]
  set outJets_1_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_1_0 ]
  set outJets_2_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_2_0 ]
  set outJets_3_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_3_0 ]
  set outJets_4_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_4_0 ]
  set outJets_5_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_5_0 ]
  set outJets_6_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_6_0 ]
  set outJets_7_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_7_0 ]
  set outJets_8_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_8_0 ]
  set outJets_9_0 [ create_bd_port -dir O -from 63 -to 0 -type data outJets_9_0 ]

  # Create instance: hls_histogram_buffer_0, and set properties
  set hls_histogram_buffer_0 [ create_bd_cell -type ip -vlnv cern-cms:hls:hls_histogram_buffer:0.1 hls_histogram_buffer_0 ]

  # Create instance: hls_histogrammer_0, and set properties
  set hls_histogrammer_0 [ create_bd_cell -type ip -vlnv cern-cms:hls:hls_histogrammer:0.2 hls_histogrammer_0 ]

  # Create instance: hls_jet_clustering_0, and set properties
  set hls_jet_clustering_0 [ create_bd_cell -type ip -vlnv cern-cms:hls:hls_jet_clustering:0.2 hls_jet_clustering_0 ]

  # Create port connections
  connect_bd_net -net barrel_inputs_0_0_1 [get_bd_ports barrel_inputs_0_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_0]
  connect_bd_net -net barrel_inputs_10_0_1 [get_bd_ports barrel_inputs_10_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_10]
  connect_bd_net -net barrel_inputs_11_0_1 [get_bd_ports barrel_inputs_11_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_11]
  connect_bd_net -net barrel_inputs_12_0_1 [get_bd_ports barrel_inputs_12_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_12]
  connect_bd_net -net barrel_inputs_13_0_1 [get_bd_ports barrel_inputs_13_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_13]
  connect_bd_net -net barrel_inputs_14_0_1 [get_bd_ports barrel_inputs_14_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_14]
  connect_bd_net -net barrel_inputs_15_0_1 [get_bd_ports barrel_inputs_15_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_15]
  connect_bd_net -net barrel_inputs_16_0_1 [get_bd_ports barrel_inputs_16_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_16]
  connect_bd_net -net barrel_inputs_17_0_1 [get_bd_ports barrel_inputs_17_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_17]
  connect_bd_net -net barrel_inputs_18_0_1 [get_bd_ports barrel_inputs_18_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_18]
  connect_bd_net -net barrel_inputs_19_0_1 [get_bd_ports barrel_inputs_19_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_19]
  connect_bd_net -net barrel_inputs_1_0_1 [get_bd_ports barrel_inputs_1_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_1]
  connect_bd_net -net barrel_inputs_20_0_1 [get_bd_ports barrel_inputs_20_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_20]
  connect_bd_net -net barrel_inputs_21_0_1 [get_bd_ports barrel_inputs_21_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_21]
  connect_bd_net -net barrel_inputs_22_0_1 [get_bd_ports barrel_inputs_22_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_22]
  connect_bd_net -net barrel_inputs_23_0_1 [get_bd_ports barrel_inputs_23_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_23]
  connect_bd_net -net barrel_inputs_2_0_1 [get_bd_ports barrel_inputs_2_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_2]
  connect_bd_net -net barrel_inputs_3_0_1 [get_bd_ports barrel_inputs_3_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_3]
  connect_bd_net -net barrel_inputs_4_0_1 [get_bd_ports barrel_inputs_4_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_4]
  connect_bd_net -net barrel_inputs_5_0_1 [get_bd_ports barrel_inputs_5_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_5]
  connect_bd_net -net barrel_inputs_6_0_1 [get_bd_ports barrel_inputs_6_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_6]
  connect_bd_net -net barrel_inputs_7_0_1 [get_bd_ports barrel_inputs_7_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_7]
  connect_bd_net -net barrel_inputs_8_0_1 [get_bd_ports barrel_inputs_8_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_8]
  connect_bd_net -net barrel_inputs_9_0_1 [get_bd_ports barrel_inputs_9_0] [get_bd_pins hls_histogrammer_0/barrel_inputs_9]
  connect_bd_net -net hls_histogram_buffer_0_outBins_0_V [get_bd_pins hls_histogram_buffer_0/outBins_0_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_0_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_10_V [get_bd_pins hls_histogram_buffer_0/outBins_10_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_10_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_11_V [get_bd_pins hls_histogram_buffer_0/outBins_11_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_11_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_12_V [get_bd_pins hls_histogram_buffer_0/outBins_12_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_12_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_13_V [get_bd_pins hls_histogram_buffer_0/outBins_13_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_13_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_14_V [get_bd_pins hls_histogram_buffer_0/outBins_14_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_14_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_15_V [get_bd_pins hls_histogram_buffer_0/outBins_15_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_15_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_16_V [get_bd_pins hls_histogram_buffer_0/outBins_16_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_16_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_17_V [get_bd_pins hls_histogram_buffer_0/outBins_17_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_17_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_1_V [get_bd_pins hls_histogram_buffer_0/outBins_1_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_1_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_2_V [get_bd_pins hls_histogram_buffer_0/outBins_2_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_2_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_3_V [get_bd_pins hls_histogram_buffer_0/outBins_3_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_3_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_4_V [get_bd_pins hls_histogram_buffer_0/outBins_4_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_4_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_5_V [get_bd_pins hls_histogram_buffer_0/outBins_5_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_5_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_6_V [get_bd_pins hls_histogram_buffer_0/outBins_6_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_6_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_7_V [get_bd_pins hls_histogram_buffer_0/outBins_7_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_7_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_8_V [get_bd_pins hls_histogram_buffer_0/outBins_8_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_8_V]
  connect_bd_net -net hls_histogram_buffer_0_outBins_9_V [get_bd_pins hls_histogram_buffer_0/outBins_9_V] [get_bd_pins hls_jet_clustering_0/inCaloGridPhiSlice_9_V]
  connect_bd_net -net hls_histogram_buffer_0_outReset [get_bd_pins hls_histogram_buffer_0/outValid] [get_bd_pins hls_jet_clustering_0/d0Valid]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_0_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_1_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_2_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_3_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_4_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_5_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_6_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_0_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_1_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_2_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_3_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_4_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_5_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_6_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_7_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_8_V [get_bd_pins hls_histogram_buffer_0/inBarrelBins_7_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_8_V]
  connect_bd_net -net hls_histogrammer_0_outReset [get_bd_pins hls_histogram_buffer_0/d0Valid] [get_bd_pins hls_histogrammer_0/outValid]
  connect_bd_net -net hls_jet_clustering_0_outJets_0 [get_bd_ports outJets_0_0] [get_bd_pins hls_jet_clustering_0/outJets_0]
  connect_bd_net -net hls_jet_clustering_0_outJets_1 [get_bd_ports outJets_1_0] [get_bd_pins hls_jet_clustering_0/outJets_1]
  connect_bd_net -net hls_jet_clustering_0_outJets_2 [get_bd_ports outJets_2_0] [get_bd_pins hls_jet_clustering_0/outJets_2]
  connect_bd_net -net hls_jet_clustering_0_outJets_3 [get_bd_ports outJets_3_0] [get_bd_pins hls_jet_clustering_0/outJets_3]
  connect_bd_net -net hls_jet_clustering_0_outJets_4 [get_bd_ports outJets_4_0] [get_bd_pins hls_jet_clustering_0/outJets_4]
  connect_bd_net -net hls_jet_clustering_0_outJets_5 [get_bd_ports outJets_5_0] [get_bd_pins hls_jet_clustering_0/outJets_5]
  connect_bd_net -net hls_jet_clustering_0_outJets_6 [get_bd_ports outJets_6_0] [get_bd_pins hls_jet_clustering_0/outJets_6]
  connect_bd_net -net hls_jet_clustering_0_outJets_7 [get_bd_ports outJets_7_0] [get_bd_pins hls_jet_clustering_0/outJets_7]
  connect_bd_net -net hls_jet_clustering_0_outJets_8 [get_bd_ports outJets_8_0] [get_bd_pins hls_jet_clustering_0/outJets_8]
  connect_bd_net -net hls_jet_clustering_0_outJets_9 [get_bd_ports outJets_9_0] [get_bd_pins hls_jet_clustering_0/outJets_9]
  connect_bd_net -net hls_jet_clustering_0_outJets_10 [get_bd_ports outJets_10_0] [get_bd_pins hls_jet_clustering_0/outJets_10]
  connect_bd_net -net hls_jet_clustering_0_outJets_11 [get_bd_ports outJets_11_0] [get_bd_pins hls_jet_clustering_0/outJets_11]
  connect_bd_net -net hls_jet_clustering_0_outJets_12 [get_bd_ports outJets_12_0] [get_bd_pins hls_jet_clustering_0/outJets_12]
  connect_bd_net -net hls_jet_clustering_0_outJets_13 [get_bd_ports outJets_13_0] [get_bd_pins hls_jet_clustering_0/outJets_13]
  connect_bd_net -net hls_jet_clustering_0_outJets_14 [get_bd_ports outJets_14_0] [get_bd_pins hls_jet_clustering_0/outJets_14]
  connect_bd_net -net hls_jet_clustering_0_outJets_15 [get_bd_ports outJets_15_0] [get_bd_pins hls_jet_clustering_0/outJets_15]
  connect_bd_net -net hls_jet_clustering_0_outJets_16 [get_bd_ports outJets_16_0] [get_bd_pins hls_jet_clustering_0/outJets_16]
  connect_bd_net -net hls_jet_clustering_0_outJets_17 [get_bd_ports outJets_17_0] [get_bd_pins hls_jet_clustering_0/outJets_17]
  connect_bd_net -net d0Valid_0_1 [get_bd_ports d0Valid_0] [get_bd_pins hls_histogrammer_0/d0Valid]
  connect_bd_net -net in_clock_240MHz_1 [get_bd_ports in_clock_240MHz] [get_bd_pins hls_histogram_buffer_0/ap_clk] [get_bd_pins hls_histogrammer_0/ap_clk] [get_bd_pins hls_jet_clustering_0/ap_clk]

  set barrel_bins_0_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_0_V ]
  set barrel_bins_0_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_1_V ]
  set barrel_bins_0_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_2_V ]
  set barrel_bins_0_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_3_V ]
  set barrel_bins_0_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_4_V ]
  set barrel_bins_0_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_5_V ]
  set barrel_bins_0_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_6_V ]
  set barrel_bins_0_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_7_V ]
  set barrel_bins_0_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_0_8_V ]
  set barrel_bins_1_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_0_V ]
  set barrel_bins_1_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_1_V ]
  set barrel_bins_1_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_2_V ]
  set barrel_bins_1_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_3_V ]
  set barrel_bins_1_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_4_V ]
  set barrel_bins_1_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_5_V ]
  set barrel_bins_1_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_6_V ]
  set barrel_bins_1_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_7_V ]
  set barrel_bins_1_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_1_8_V ]
  set barrel_bins_2_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_0_V ]
  set barrel_bins_2_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_1_V ]
  set barrel_bins_2_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_2_V ]
  set barrel_bins_2_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_3_V ]
  set barrel_bins_2_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_4_V ]
  set barrel_bins_2_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_5_V ]
  set barrel_bins_2_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_6_V ]
  set barrel_bins_2_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_7_V ]
  set barrel_bins_2_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_2_8_V ]
  set barrel_bins_3_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_0_V ]
  set barrel_bins_3_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_1_V ]
  set barrel_bins_3_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_2_V ]
  set barrel_bins_3_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_3_V ]
  set barrel_bins_3_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_4_V ]
  set barrel_bins_3_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_5_V ]
  set barrel_bins_3_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_6_V ]
  set barrel_bins_3_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_7_V ]
  set barrel_bins_3_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_3_8_V ]
  set barrel_bins_4_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_0_V ]
  set barrel_bins_4_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_1_V ]
  set barrel_bins_4_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_2_V ]
  set barrel_bins_4_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_3_V ]
  set barrel_bins_4_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_4_V ]
  set barrel_bins_4_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_5_V ]
  set barrel_bins_4_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_6_V ]
  set barrel_bins_4_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_7_V ]
  set barrel_bins_4_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_4_8_V ]
  set barrel_bins_5_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_0_V ]
  set barrel_bins_5_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_1_V ]
  set barrel_bins_5_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_2_V ]
  set barrel_bins_5_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_3_V ]
  set barrel_bins_5_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_4_V ]
  set barrel_bins_5_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_5_V ]
  set barrel_bins_5_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_6_V ]
  set barrel_bins_5_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_7_V ]
  set barrel_bins_5_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_5_8_V ]
  set barrel_bins_6_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_0_V ]
  set barrel_bins_6_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_1_V ]
  set barrel_bins_6_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_2_V ]
  set barrel_bins_6_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_3_V ]
  set barrel_bins_6_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_4_V ]
  set barrel_bins_6_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_5_V ]
  set barrel_bins_6_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_6_V ]
  set barrel_bins_6_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_7_V ]
  set barrel_bins_6_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_6_8_V ]
  set barrel_bins_7_0_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_0_V ]
  set barrel_bins_7_1_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_1_V ]
  set barrel_bins_7_2_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_2_V ]
  set barrel_bins_7_3_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_3_V ]
  set barrel_bins_7_4_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_4_V ]
  set barrel_bins_7_5_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_5_V ]
  set barrel_bins_7_6_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_6_V ]
  set barrel_bins_7_7_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_7_V ]
  set barrel_bins_7_8_V [ create_bd_port -dir O -from 15 -to 0 -type data barrel_bins_7_8_V ]
  set hls_histogrammer_outValid [ create_bd_port -dir O -from 1 -to 0 -type data hls_histogrammer_outValid ]

  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_0_V [get_bd_ports barrel_bins_0_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_1_V [get_bd_ports barrel_bins_0_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_2_V [get_bd_ports barrel_bins_0_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_3_V [get_bd_ports barrel_bins_0_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_4_V [get_bd_ports barrel_bins_0_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_5_V [get_bd_ports barrel_bins_0_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_6_V [get_bd_ports barrel_bins_0_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_7_V [get_bd_ports barrel_bins_0_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_0_8_V [get_bd_ports barrel_bins_0_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_0_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_0_V [get_bd_ports barrel_bins_1_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_1_V [get_bd_ports barrel_bins_1_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_2_V [get_bd_ports barrel_bins_1_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_3_V [get_bd_ports barrel_bins_1_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_4_V [get_bd_ports barrel_bins_1_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_5_V [get_bd_ports barrel_bins_1_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_6_V [get_bd_ports barrel_bins_1_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_7_V [get_bd_ports barrel_bins_1_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_1_8_V [get_bd_ports barrel_bins_1_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_1_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_0_V [get_bd_ports barrel_bins_2_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_1_V [get_bd_ports barrel_bins_2_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_2_V [get_bd_ports barrel_bins_2_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_3_V [get_bd_ports barrel_bins_2_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_4_V [get_bd_ports barrel_bins_2_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_5_V [get_bd_ports barrel_bins_2_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_6_V [get_bd_ports barrel_bins_2_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_7_V [get_bd_ports barrel_bins_2_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_2_8_V [get_bd_ports barrel_bins_2_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_2_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_0_V [get_bd_ports barrel_bins_3_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_1_V [get_bd_ports barrel_bins_3_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_2_V [get_bd_ports barrel_bins_3_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_3_V [get_bd_ports barrel_bins_3_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_4_V [get_bd_ports barrel_bins_3_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_5_V [get_bd_ports barrel_bins_3_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_6_V [get_bd_ports barrel_bins_3_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_7_V [get_bd_ports barrel_bins_3_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_3_8_V [get_bd_ports barrel_bins_3_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_3_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_0_V [get_bd_ports barrel_bins_4_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_1_V [get_bd_ports barrel_bins_4_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_2_V [get_bd_ports barrel_bins_4_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_3_V [get_bd_ports barrel_bins_4_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_4_V [get_bd_ports barrel_bins_4_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_5_V [get_bd_ports barrel_bins_4_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_6_V [get_bd_ports barrel_bins_4_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_7_V [get_bd_ports barrel_bins_4_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_4_8_V [get_bd_ports barrel_bins_4_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_4_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_0_V [get_bd_ports barrel_bins_5_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_1_V [get_bd_ports barrel_bins_5_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_2_V [get_bd_ports barrel_bins_5_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_3_V [get_bd_ports barrel_bins_5_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_4_V [get_bd_ports barrel_bins_5_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_5_V [get_bd_ports barrel_bins_5_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_6_V [get_bd_ports barrel_bins_5_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_7_V [get_bd_ports barrel_bins_5_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_5_8_V [get_bd_ports barrel_bins_5_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_5_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_0_V [get_bd_ports barrel_bins_6_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_1_V [get_bd_ports barrel_bins_6_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_2_V [get_bd_ports barrel_bins_6_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_3_V [get_bd_ports barrel_bins_6_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_4_V [get_bd_ports barrel_bins_6_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_5_V [get_bd_ports barrel_bins_6_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_6_V [get_bd_ports barrel_bins_6_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_7_V [get_bd_ports barrel_bins_6_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_6_8_V [get_bd_ports barrel_bins_6_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_6_8_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_0_V [get_bd_ports barrel_bins_7_0_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_0_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_1_V [get_bd_ports barrel_bins_7_1_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_1_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_2_V [get_bd_ports barrel_bins_7_2_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_2_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_3_V [get_bd_ports barrel_bins_7_3_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_3_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_4_V [get_bd_ports barrel_bins_7_4_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_4_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_5_V [get_bd_ports barrel_bins_7_5_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_5_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_6_V [get_bd_ports barrel_bins_7_6_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_6_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_7_V [get_bd_ports barrel_bins_7_7_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_7_V]
  connect_bd_net -net hls_histogrammer_0_barrel_bins_7_8_V [get_bd_ports barrel_bins_7_8_V] [get_bd_pins hls_histogrammer_0/barrel_bins_7_8_V]
  connect_bd_net [get_bd_ports hls_histogrammer_outValid] [get_bd_pins hls_histogrammer_0/outValid]


  set buffer_outBins_0_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_0_V ]
  set buffer_outBins_10_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_10_V ]
  set buffer_outBins_11_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_11_V ]
  set buffer_outBins_12_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_12_V ]
  set buffer_outBins_13_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_13_V ]
  set buffer_outBins_14_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_14_V ]
  set buffer_outBins_15_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_15_V ]
  set buffer_outBins_16_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_16_V ]
  set buffer_outBins_17_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_17_V ]
  set buffer_outBins_1_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_1_V ]
  set buffer_outBins_2_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_2_V ]
  set buffer_outBins_3_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_3_V ]
  set buffer_outBins_4_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_4_V ]
  set buffer_outBins_5_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_5_V ]
  set buffer_outBins_6_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_6_V ]
  set buffer_outBins_7_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_7_V ]
  set buffer_outBins_8_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_8_V ]
  set buffer_outBins_9_V [ create_bd_port -dir O -from 15 -to 0 -type data outBins_9_V ]
  set hls_histogram_buffer_outValid [ create_bd_port -dir O -from 1 -to 0 -type data hls_histogram_buffer_outValid ]

  connect_bd_net -net hls_histogram_buffer_0_outBins_0_V [get_bd_ports outBins_0_V] [get_bd_pins hls_histogram_buffer_0/outBins_0_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_10_V [get_bd_ports outBins_10_V] [get_bd_pins hls_histogram_buffer_0/outBins_10_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_11_V [get_bd_ports outBins_11_V] [get_bd_pins hls_histogram_buffer_0/outBins_11_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_12_V [get_bd_ports outBins_12_V] [get_bd_pins hls_histogram_buffer_0/outBins_12_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_13_V [get_bd_ports outBins_13_V] [get_bd_pins hls_histogram_buffer_0/outBins_13_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_14_V [get_bd_ports outBins_14_V] [get_bd_pins hls_histogram_buffer_0/outBins_14_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_15_V [get_bd_ports outBins_15_V] [get_bd_pins hls_histogram_buffer_0/outBins_15_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_16_V [get_bd_ports outBins_16_V] [get_bd_pins hls_histogram_buffer_0/outBins_16_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_17_V [get_bd_ports outBins_17_V] [get_bd_pins hls_histogram_buffer_0/outBins_17_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_1_V [get_bd_ports outBins_1_V] [get_bd_pins hls_histogram_buffer_0/outBins_1_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_2_V [get_bd_ports outBins_2_V] [get_bd_pins hls_histogram_buffer_0/outBins_2_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_3_V [get_bd_ports outBins_3_V] [get_bd_pins hls_histogram_buffer_0/outBins_3_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_4_V [get_bd_ports outBins_4_V] [get_bd_pins hls_histogram_buffer_0/outBins_4_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_5_V [get_bd_ports outBins_5_V] [get_bd_pins hls_histogram_buffer_0/outBins_5_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_6_V [get_bd_ports outBins_6_V] [get_bd_pins hls_histogram_buffer_0/outBins_6_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_7_V [get_bd_ports outBins_7_V] [get_bd_pins hls_histogram_buffer_0/outBins_7_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_8_V [get_bd_ports outBins_8_V] [get_bd_pins hls_histogram_buffer_0/outBins_8_V] 
  connect_bd_net -net hls_histogram_buffer_0_outBins_9_V [get_bd_ports outBins_9_V] [get_bd_pins hls_histogram_buffer_0/outBins_9_V] 
  connect_bd_net [get_bd_ports hls_histogram_buffer_outValid] [get_bd_pins hls_histogram_buffer_0/outValid] 

  # Create address segments


  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""

common::send_msg_id "BD_TCL-1000" "WARNING" "This Tcl script was generated from a block design that has not been validated. It is possible that design <$design_name> may result in errors during validation."

# synthetising block design and creating hdl wrapper
update_compile_order -fileset sources_1
make_wrapper -files [get_files jet_trigger_chain/jet_trigger_chain.srcs/sources_1/bd/jet_trigger_chain/jet_trigger_chain.bd] -top
add_files -norecurse jet_trigger_chain/jet_trigger_chain.srcs/sources_1/bd/jet_trigger_chain/hdl/jet_trigger_chain_wrapper.vhd
set_property synth_checkpoint_mode Singular [get_files  jet_trigger_chain/jet_trigger_chain.srcs/sources_1/bd/jet_trigger_chain/jet_trigger_chain.bd]
generate_target all [get_files  jet_trigger_chain/jet_trigger_chain.srcs/sources_1/bd/jet_trigger_chain/jet_trigger_chain.bd]
export_ip_user_files -of_objects [get_files jet_trigger_chain/jet_trigger_chain.srcs/sources_1/bd/jet_trigger_chain/jet_trigger_chain.bd] -no_script -sync -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] jet_trigger_chain/jet_trigger_chain.srcs/sources_1/bd/jet_trigger_chain/jet_trigger_chain.bd]
launch_runs -jobs 4 jet_trigger_chain_synth_1
export_simulation -of_objects [get_files jet_trigger_chain/jet_trigger_chain.srcs/sources_1/bd/jet_trigger_chain/jet_trigger_chain.bd] -directory jet_trigger_chain/jet_trigger_chain.ip_user_files/sim_scripts -ip_user_files_dir jet_trigger_chain/jet_trigger_chain.ip_user_files -ipstatic_source_dir jet_trigger_chain/jet_trigger_chain.ip_user_files/ipstatic -lib_map_path [list {modelsim=jet_trigger_chain/jet_trigger_chain.cache/compile_simlib/modelsim} {questa=jet_trigger_chain/jet_trigger_chain.cache/compile_simlib/questa} {ies=jet_trigger_chain/jet_trigger_chain.cache/compile_simlib/ies} {xcelium=jet_trigger_chain/jet_trigger_chain.cache/compile_simlib/xcelium} {vcs=jet_trigger_chain/jet_trigger_chain.cache/compile_simlib/vcs} {riviera=jet_trigger_chain/jet_trigger_chain.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet

ipx::package_project -root_dir IPs/jet_trigger_chain -vendor cern-cms -library hls -taxonomy /UserIP -generated_files -import_files -set_current true
ipx::archive_core IPs/jet_trigger_chain/cern-cms_hls_jet_trigger_chain_wrapper_1.0.zip [ipx::current_core]