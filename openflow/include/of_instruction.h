/*Copyright 2013 haifeng.zhou, email: haifeng.zhou@live.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

/*A.2.4 Flow Instruction Structures
Flow instructions associated with a 
ow table entry are executed when a 
ow matches the entry. The
list of instructions that are currently dened are:*/
enum ofp_instruction_type {
    OFPIT_GOTO_TABLE = 1, /* Setup the next table in the lookup
    pipeline */
    OFPIT_WRITE_METADATA = 2, /* Setup the metadata field for use later in
    pipeline */
    OFPIT_WRITE_ACTIONS = 3, /* Write the action(s) onto the datapath action
    set */
    OFPIT_APPLY_ACTIONS = 4, /* Applies the action(s) immediately */
    OFPIT_CLEAR_ACTIONS = 5, /* Clears all actions from the datapath
    action set */
    OFPIT_METER = 6, /* Apply meter (rate limiter) */
    OFPIT_EXPERIMENTER = 0xFFFF /* Experimenter instruction */
};

/*The instruction set is described in section 5.9. Flow tables may support a subset of instruction types.
An instruction denition contains the instruction type, length, and any associated data:*/

/* Instruction header that is common to all instructions. The length includes
* the header and any padding used to make the instruction 64-bit aligned.
* NB: The length of an instruction *must* always be a multiple of eight. */
struct ofp_instruction {
    uint16_t type; /* Instruction type */
    uint16_t len; /* Length of this struct in bytes. */
};
OFP_ASSERT(sizeof(struct ofp_instruction) == 4);

/*The OFPIT_GOTO_TABLE instruction uses the following structure and elds:*/
/* Instruction structure for OFPIT_GOTO_TABLE */
struct ofp_instruction_goto_table {
    uint16_t type; /* OFPIT_GOTO_TABLE */
    uint16_t len; /* Length of this struct in bytes. */
    uint8_t table_id; /* Set next table in the lookup pipeline */
    uint8_t pad[3]; /* Pad to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_instruction_goto_table) == 8);
/*table_id indicates the next table in the packet processing pipeline.
The OFPIT_WRITE_METADATA instruction uses the following structure and elds:*/

/* Instruction structure for OFPIT_WRITE_METADATA */
struct ofp_instruction_write_metadata {
    uint16_t type; /* OFPIT_WRITE_METADATA */
    uint16_t len; /* Length of this struct in bytes. */
    uint8_t pad[4]; /* Align to 64-bits */
    uint64_t metadata; /* Metadata value to write */
    uint64_t metadata_mask; /* Metadata write bitmask */
};
OFP_ASSERT(sizeof(struct ofp_instruction_write_metadata) == 24);
/*Metadata for the next table lookup can be written using the metadata and the metadata_mask in
order to set specic bits on the match eld. If this instruction is not specied, the metadata is passed,
unchanged.
The OFPIT_WRITE_ACTIONS, OFPIT_APPLY_ACTIONS, and OFPIT_CLEAR_ACTIONS instructions use the
following structure and elds:*/
/* Instruction structure for OFPIT_WRITE/APPLY/CLEAR_ACTIONS */
struct ofp_instruction_actions {
    uint16_t type; /* One of OFPIT_*_ACTIONS */
    uint16_t len; /* Length of this struct in bytes. */
    uint8_t pad[4]; /* Align to 64-bits */
    struct ofp_action_header actions[0]; /* Actions associated with
    OFPIT_WRITE_ACTIONS and
    OFPIT_APPLY_ACTIONS */
};
OFP_ASSERT(sizeof(struct ofp_instruction_actions) == 8);

/*For the Apply-Actions instruction, the actions eld is treated as a list and the actions are applied to
the packet in-order. For the Write-Actions instruction, the actions eld is treated as a set and the
actions are merged into the current action set.
For the Clear-Actions instruction, the structure does not contain any actions.
The OFPIT_METER instruction uses the following structure and elds:*/

/* Instruction structure for OFPIT_METER */
struct ofp_instruction_meter {
    uint16_t type; /* OFPIT_METER */
    uint16_t len; /* Length is 8. */
    uint32_t meter_id; /* Meter instance. */
};
OFP_ASSERT(sizeof(struct ofp_instruction_meter) == 8);

/*meter_id indicates which meter to apply on the packet.
An OFPIT_EXPERIMENTER instruction uses the following structure and elds:*/

/* Instruction structure for experimental instructions */
struct ofp_instruction_experimenter {
    uint16_t type; /* OFPIT_EXPERIMENTER */
    uint16_t len; /* Length of this struct in bytes */
    uint32_t experimenter; /* Experimenter ID which takes the same form
    as in struct ofp_experimenter_header. */
    /* Experimenter-defined arbitrary additional data. */
};
OFP_ASSERT(sizeof(struct ofp_instruction_experimenter) == 8);
/*The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).*/

