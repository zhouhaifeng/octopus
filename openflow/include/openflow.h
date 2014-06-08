/*Copyright (c) 2013 haifeng.zhou, email: haifeng.zhou@live.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/


/*A.3 Controller-to-Switch Messages
A.3.1 Handshake
Upon session establishment, the controller sends an OFPT_FEATURES_REQUEST message. This message
does not contain a body beyond the OpenFlow header. The switch responds with an
OFPT_FEATURES_REPLY message:*/

/* Switch features. */
struct ofp_switch_features {
    struct ofp_header header;
    uint64_t datapath_id; /* Datapath unique ID. The lower 48-bits are for
    a MAC address, while the upper 16-bits are
    implementer-defined. */
    uint32_t n_buffers; /* Max packets buffered at once. */
    uint8_t n_tables; /* Number of tables supported by datapath. */
    uint8_t auxiliary_id; /* Identify auxiliary connections */
    uint8_t pad[2]; /* Align to 64-bits. */
    /* Features. */
    uint32_t capabilities; /* Bitmap of support "ofp_capabilities". */
    uint32_t reserved;
};
OFP_ASSERT(sizeof(struct ofp_switch_features) == 32);
/*The datapath_id eld uniquely identies a datapath. The lower 48 bits are intended for the switch
MAC address, while the top 16 bits are up to the implementer. An example use of the top 16 bits would
be a VLAN ID to distinguish multiple virtual switch instances on a single physical switch. This eld
should be treated as an opaque bit string by controllers.
The n_buffers eld species the maximum number of packets the switch can buer when sending
packets to the controller using packet-in messages (see 6.1.2).
The n_tables eld describes the number of tables supported by the switch, each of which can have a
dierent set of supported match elds, actions and number of entries. When the controller and switch
61 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
rst communicate, the controller will nd out how many tables the switch supports from the Features
Reply. If it wishes to understand the size, types, and order in which tables are consulted, the controller
sends a OFPMP_TABLE_FEATURES multipart request (see A.3.5.5). A switch must return these tables in
the order the packets traverse the tables.
The auxiliary_id eld identify the type of connection from the switch to the controller, the main
connection has this eld set to zero, an auxiliary connection has this eld set to a non-zero value (see
6.3.5).
The capabilities eld uses the following 
ags:*/

/* Capabilities supported by the datapath. */
enum ofp_capabilities {
    OFPC_FLOW_STATS = 1 << 0, /* Flow statistics. */
    OFPC_TABLE_STATS = 1 << 1, /* Table statistics. */
    OFPC_PORT_STATS = 1 << 2, /* Port statistics. */
    OFPC_GROUP_STATS = 1 << 3, /* Group statistics. */
    OFPC_IP_REASM = 1 << 5, /* Can reassemble IP fragments. */
    OFPC_QUEUE_STATS = 1 << 6, /* Queue statistics. */
    OFPC_PORT_BLOCKED = 1 << 8 /* Switch will block looping ports. */
};

/*The OFPC_PORT_BLOCKED bit indicates that a switch protocol outside of OpenFlow, such as 802.1D
Spanning Tree, will detect topology loops and block ports to prevent packet loops. If this bit is not set,
in most cases the controller should implement a mechanism to prevent packet loops.
A.3.2 Switch Conguration
The controller is able to set and query conguration parameters in the switch with the OFPT_SET_CONFIG
and OFPT_GET_CONFIG_REQUEST messages, respectively. The switch responds to a conguration request
with an OFPT_GET_CONFIG_REPLY message; it does not reply to a request to set the conguration.
There is no body for OFPT_GET_CONFIG_REQUEST beyond the OpenFlow header. The OFPT_SET_CONFIG
and OFPT_GET_CONFIG_REPLY use the following:*/

/* Switch configuration. */
struct ofp_switch_config {
    struct ofp_header header;
    uint16_t flags; /* OFPC_* flags. */
    uint16_t miss_send_len; /* Max bytes of packet that datapath
    should send to the controller. See
    ofp_controller_max_len for valid values.
    */
};
OFP_ASSERT(sizeof(struct ofp_switch_config) == 12);

/*The conguration ags include the following:*/
enum ofp_config_flags {
    /* Handling of IP fragments. */
    OFPC_FRAG_NORMAL = 0, /* No special handling for fragments. */
    OFPC_FRAG_DROP = 1 << 0, /* Drop fragments. */
    OFPC_FRAG_REASM = 1 << 1, /* Reassemble (only if OFPC_IP_REASM set). */
    OFPC_FRAG_MASK = 3,
};

/*The OFPC_FRAG_* 
ags indicate whether IP fragments should be treated normally, dropped, or reassembled.
\Normal" handling of fragments means that an attempt should be made to pass the fragments
through the OpenFlow tables. If any eld is not present (e.g., the TCP/UDP ports didn't t), then the
packet should not match any entry that has that eld set.
The miss_send_len eld denes the number of bytes of each packet sent to the controller by the
OpenFlow pipeline when not using an output action to the OFPP_CONTROLLER logical port, for example
sending packets with invalid TTL if this message reason is enabled. If this eld equals 0, the switch must
send zero bytes of the packet in the ofp_packet_in message. If the value is set to OFPCML_NO_BUFFER
the complete packet must be included in the message, and should not be buered.*/
/*A.3.3 Flow Table Conguration
Flow tables are numbered from 0 and can take any number until OFPTT_MAX. OFPTT_ALL is a reserved
value.*/
/* Table numbering. Tables can use any number up to OFPT_MAX. */
enum ofp_table {
    /* Last usable table number. */
    OFPTT_MAX = 0xfe,
    /* Fake tables. */
    OFPTT_ALL = 0xff /* Wildcard table used for table config,
    flow stats and flow deletes. */
};

/*The controller can congure and query table state in the switch with the OFP_TABLE_MOD and
OFPMP_TABLE_STATS requests, respectively. The switch responds to a table multipart request with
a OFPT_MULTIPART_REPLY message. The OFP_TABLE_MOD use the following structure and elds:*/
/* Configure/Modify behavior of a flow table */
struct ofp_table_mod {
    struct ofp_header header;
    uint8_t table_id; /* ID of the table, OFPTT_ALL indicates all tables */
    uint8_t pad[3]; /* Pad to 32 bits */
    uint32_t config; /* Bitmap of OFPTC_* flags */
};

OFP_ASSERT(sizeof(struct ofp_table_mod) == 16);

/*The table_id chooses the table to which the conguration change should be applied. If the table_id
is OFPTT_ALL, the conguration is applied to all tables in the switch.
63 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The config eld is a bitmap that is provided for backward compatibility with earlier version of the
specication, it is reserved for future use. There is no 
ags that are currently dened for that eld. The
following values are dened for that eld :*/
/* Flags to configure the table. Reserved for future use. */
enum ofp_table_config {
    OFPTC_DEPRECATED_MASK = 3, /* Deprecated bits */
};

/*A.3.4 Modify State Messages
A.3.4.1 Modify Flow Entry Message
Modications to a 
ow table from the controller are done with the OFPT_FLOW_MOD message:*/
/* Flow setup and teardown (controller -> datapath). */
struct ofp_flow_mod {
    struct ofp_header header;
    uint64_t cookie; /* Opaque controller-issued identifier. */
    uint64_t cookie_mask; /* Mask used to restrict the cookie bits
    that must match when the command is
    OFPFC_MODIFY* or OFPFC_DELETE*. A value
    of 0 indicates no restriction. */
    /* Flow actions. */
    uint8_t table_id; /* ID of the table to put the flow in.
    For OFPFC_DELETE_* commands, OFPTT_ALL
    can also be used to delete matching
    flows from all tables. */
    uint8_t command; /* One of OFPFC_*. */
    uint16_t idle_timeout; /* Idle time before discarding (seconds). */
    uint16_t hard_timeout; /* Max time before discarding (seconds). */
    uint16_t priority; /* Priority level of flow entry. */
    uint32_t buffer_id; /* Buffered packet to apply to, or
    OFP_NO_BUFFER.
    Not meaningful for OFPFC_DELETE*. */
    uint32_t out_port; /* For OFPFC_DELETE* commands, require
    matching entries to include this as an
    output port. A value of OFPP_ANY
    indicates no restriction. */
    uint32_t out_group; /* For OFPFC_DELETE* commands, require
    matching entries to include this as an
    output group. A value of OFPG_ANY
    indicates no restriction. */
    uint16_t flags; /* One of OFPFF_*. */
    uint8_t pad[2];
    struct ofp_match match; /* Fields to match. Variable size. */
    //struct ofp_instruction instructions[0]; /* Instruction set */
};
OFP_ASSERT(sizeof(struct ofp_flow_mod) == 56);

/*The cookie eld is an opaque data value chosen by the controller. This value appears in 
ow removed
messages and 
ow statistics, and can also be used to lter 
ow statistics, 
ow modication and 
ow
deletion (see 6.4). It is not used by the packet processing pipeline, and thus does not need to reside in
hardware. The value -1 (0x) is reserved and must not be used. When a 
ow entry is inserted
in a table through an OFPFC_ADD message, its cookie eld is set to the provided value. When a 
ow
entry is modied (OFPFC_MODIFY or OFPFC_MODIFY_STRICT messages), its cookie eld is unchanged.
If the cookie_mask eld is non-zero, it is used with the cookie eld to restrict 
ow matching while
modifying or deleting 
ow entries. This eld is ignored by OFPFC_ADD messages. The cookie_mask
eld's behavior is explained in Section 6.4.
The table_id eld species the table into which the 
ow entry should be inserted, modied or deleted.
Table 0 signies the rst table in the pipeline. The use of OFPTT_ALL is only valid for delete requests.
The command eld must be one of the following:*/
enum ofp_flow_mod_command {
    OFPFC_ADD = 0, /* New flow. */
    OFPFC_MODIFY = 1, /* Modify all matching flows. */
    OFPFC_MODIFY_STRICT = 2, /* Modify entry strictly matching wildcards and
    priority. */
    OFPFC_DELETE = 3, /* Delete all matching flows. */
    OFPFC_DELETE_STRICT = 4, /* Delete entry strictly matching wildcards and
    priority. */
};
/*The dierences between OFPFC_MODIFY and OFPFC_MODIFY_STRICT are explained in Section 6.4 and
dierences between OFPFC_DELETE and OFPFC_DELETE_STRICT are explained in Section 6.4.
The idle_timeout and hard_timeout elds control how quickly 
ow entries expire (see 5.5). When
a 
ow entry is inserted in a table, its idle_timeout and hard_timeout elds are set with the values
from the message. When a 
ow entry is modied (OFPFC_MODIFY or OFPFC_MODIFY_STRICT messages),
the idle_timeout and hard_timeout elds are ignored.
If the idle_timeout is set and the hard_timeout is zero, the entry must expire after idle_timeout
seconds with no received trac. If the idle_timeout is zero and the hard_timeout is set, the entry
must expire in hard_timeout seconds regardless of whether or not packets are hitting the entry. If both
idle_timeout and hard_timeout are set, the 
ow entry will timeout after idle_timeout seconds with
no trac, or hard_timeout seconds, whichever comes rst. If both idle_timeout and hard_timeout
are zero, the entry is considered permanent and will never time out. It can still be removed with a
flow_mod message of type OFPFC_DELETE.
The priority indicates priority within the specied 
ow table table. Higher numbers indicate higher
priorities. This eld is used only for OFPFC_ADD messages when matching and adding 
ow entries, and
for OFPFC_MODIFY_STRICT or OFPFC_DELETE_STRICT messages when matching 
ow entries.
The buffer_id refers to a packet buered at the switch and sent to the controller by a packet-in
message. If no buered packet is associated with the 
ow mod, it must be set to OFP_NO_BUFFER. A

ow mod that includes a valid buffer_id is eectively equivalent to sending a two-message sequence of
a 
ow mod and a packet-out to OFPP_TABLE, with the requirement that the switch must fully process
the 
ow mod before the packet out. These semantics apply regardless of the table to which the 
ow
mod refers, or the instructions contained in the 
ow mod. This eld is ignored by OFPFC_DELETE and
OFPFC_DELETE_STRICT 
ow mod messages.
65 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The out_port and out_group elds optionally lter the scope of OFPFC_DELETE and
OFPFC_DELETE_STRICT messages by output port and group. If either out_port or out_group contains
a value other than OFPP_ANY or OFPG_ANY respectively, it introduces a constraint when matching.
This constraint is that the 
ow entry must contain an output action directed at that port or
group. Other constraints such as ofp_match structs and priorities are still used; this is purely an
additional constraint. Note that to disable output ltering, both out_port and out_group must be
set to OFPP_ANY and OFPG_ANY respectively. These elds are ignored by OFPFC_ADD, OFPFC_MODIFY or
OFPFC_MODIFY_STRICT messages.
The flags eld may include the following 
ags:*/

enum ofp_flow_mod_flags {
    OFPFF_SEND_FLOW_REM = 1 << 0, /* Send flow removed message when flow
    * expires or is deleted. */
    OFPFF_CHECK_OVERLAP = 1 << 1, /* Check for overlapping entries first. */
    OFPFF_RESET_COUNTS = 1 << 2, /* Reset flow packet and byte counts. */
    OFPFF_NO_PKT_COUNTS = 1 << 3, /* Don't keep track of packet count. */
    OFPFF_NO_BYT_COUNTS = 1 << 4, /* Don't keep track of byte count. */
};

/*When the OFPFF_SEND_FLOW_REM 
ag is set, the switch must send a 
ow removed message when the

ow entry expires or is deleted.
When the OFPFF_CHECK_OVERLAP 
ag is set, the switch must check that there are no con
icting entries
with the same priority prior to inserting it in the 
ow table. If there is one, the 
ow mod fails and an
error message is returned (see 6.4).
When the OFPFF_NO_PKT_COUNTS 
ag is set, the switch does not need to keep track of the 
ow packet
count. When the OFPFF_NO_BYT_COUNTS 
ag is set, the switch does not need to keep track of the 
ow
byte count. Setting those 
ags may decrease the processing load on some OpenFlow switches, however
those counters may not be available in 
ow statistics and 
ow removed messages for this 
ow entry. A
switch is not required to honor those 
ags and may keep track of a 
ow count and return it despite the
corresponding 
ag being set. If a switch does not keep track of a 
ow count, the corresponding counter
is not available and must be set to the maximum eld value (see 5.8).
When a 
ow entry is inserted in a table, its flags eld is set with the values from the message. When
a 
ow entry is matched and modied (OFPFC_MODIFY or OFPFC_MODIFY_STRICT messages), the flags
eld is ignored.
The instructions eld contains the instruction set for the 
ow entry when adding or modifying entries.
If the instruction set is not valid or supported, the switch must generate an error (see 6.4).
A.3.4.2 Modify Group Entry Message
Modications to the group table from the controller are done with the OFPT_GROUP_MOD message:
/* Group setup and teardown (controller -> datapath). */
struct ofp_group_mod {
    struct ofp_header header;
    uint16_t command; /* One of OFPGC_*. */
    66 . 2012; The Open Networking Foundation
    OpenFlow Switch Specication Version 1.3.1
    uint8_t type; /* One of OFPGT_*. */
    uint8_t pad; /* Pad to 64 bits. */
    uint32_t group_id; /* Group identifier. */
    struct ofp_bucket buckets[0]; /* The length of the bucket array is inferred
    from the length field in the header. */
};
OFP_ASSERT(sizeof(struct ofp_group_mod) == 16);

/*The semantics of the type and group elds are explained in Section 6.5.
The command eld must be one of the following:*/
/* Group commands */
enum ofp_group_mod_command {
    OFPGC_ADD = 0, /* New group. */
    OFPGC_MODIFY = 1, /* Modify all matching groups. */
    OFPGC_DELETE = 2, /* Delete all matching groups. */
};

/*The type eld must be one of the following:*/
/* Group types. Values in the range [128, 255] are reserved for experimental
* use. */
enum ofp_group_type {
    OFPGT_ALL = 0, /* All (multicast/broadcast) group. */
    OFPGT_SELECT = 1, /* Select group. */
    OFPGT_INDIRECT = 2, /* Indirect group. */
    OFPGT_FF = 3, /* Fast failover group. */
};

/*The group_id eld uniquely identies a group within a switch. The following special group identiers
are dened:*/
/* Group numbering. Groups can use any number up to OFPG_MAX. */
enum ofp_group {
    /* Last usable group number. */
    OFPG_MAX = 0xffffff00,
    /* Fake groups. */
    OFPG_ALL = 0xfffffffc, /* Represents all groups for group delete
    commands. */
    OFPG_ANY = 0xffffffff /* Wildcard group used only for flow stats
    requests. Selects all flows regardless of
    group (including flows with no group).
    */
};

/*The buckets eld is an array of buckets. For Indirect group, the arrays must contain exactly one
bucket (see 5.6.1), other group types may have multiple buckets in the array. For Fast Failover group,
the bucket order does dene the bucket priorities (see 5.6.1), and the bucket order can be changed by
modifying the group (for example using a OFPT_GROUP_MOD message with command OFPGC_MODIFY).
Buckets in the array use the following struct:*/

/* Bucket for use in groups. */
struct ofp_bucket {
    uint16_t len; /* Length the bucket in bytes, including
    this header and any padding to make it
    64-bit aligned. */
    uint16_t weight; /* Relative weight of bucket. Only
    defined for select groups. */
    uint32_t watch_port; /* Port whose state affects whether this
    bucket is live. Only required for fast
    failover groups. */
    uint32_t watch_group; /* Group whose state affects whether this
    bucket is live. Only required for fast
    failover groups. */
    uint8_t pad[4];
    struct ofp_action_header actions[0]; /* The action length is inferred
    from the length field in the
    header. */
};
OFP_ASSERT(sizeof(struct ofp_bucket) == 16);

/*The weight eld is only dened for select groups, and its support is optional. The bucket's share of the
trac processed by the group is dened by the individual bucket's weight divided by the sum of the
bucket weights in the group. When a port goes down, the change in trac distribution is undened.
The precision by which a switch's packet distribution should match bucket weights is undened.
The watch_port and watch_group elds are only required for fast failover groups, and may be optionally
implemented for other group types. These elds indicate the port and/or group whose liveness controls
whether this bucket is a candidate for forwarding. For fast failover groups, the rst bucket dened is
the highest-priority bucket, and only the highest-priority live bucket is used (see 5.6.1).
The actions eld is the action set associated with the bucket. When the bucket is selected for a packet,
its action set is applied to the packet (see 5.10).
A.3.4.3 Port Modication Message
The controller uses the OFPT_PORT_MOD message to modify the behavior of the port:*/
/* Modify behavior of the physical port */
struct ofp_port_mod {
    struct ofp_header header;
    uint32_t port_no;
    uint8_t pad[4];
    uint8_t hw_addr[OFP_ETH_ALEN]; /* The hardware address is not
    configurable. This is used to
    sanity-check the request, so it must
    be the same as returned in an
    ofp_port struct. */
    uint8_t pad2[2]; /* Pad to 64 bits. */
    uint32_t config; /* Bitmap of OFPPC_* flags. */
    uint32_t mask; /* Bitmap of OFPPC_* flags to be changed. */
    uint32_t advertise; /* Bitmap of OFPPF_*. Zero all bits to prevent
    any action taking place. */
    68 . 2012; The Open Networking Foundation
    OpenFlow Switch Specication Version 1.3.1
    uint8_t pad3[4]; /* Pad to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_port_mod) == 40);

/*The mask eld is used to select bits in the config eld to change. The advertise eld has no mask;
all port features change together.*/
