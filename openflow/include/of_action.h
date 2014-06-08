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

/*A.2.5 Action Structures
A number of actions may be associated with 
ow entries, groups or packets. The currently dened
action types are:*/

enum ofp_action_type {
    OFPAT_OUTPUT = 0, /* Output to switch port. */
    OFPAT_COPY_TTL_OUT = 11, /* Copy TTL "outwards" -- from next-to-outermost
    to outermost */
    OFPAT_COPY_TTL_IN = 12, /* Copy TTL "inwards" -- from outermost to
    next-to-outermost */
    OFPAT_SET_MPLS_TTL = 15, /* MPLS TTL */
    OFPAT_DEC_MPLS_TTL = 16, /* Decrement MPLS TTL */
    OFPAT_PUSH_VLAN = 17, /* Push a new VLAN tag */
    OFPAT_POP_VLAN = 18, /* Pop the outer VLAN tag */
    OFPAT_PUSH_MPLS = 19, /* Push a new MPLS tag */
    OFPAT_POP_MPLS = 20, /* Pop the outer MPLS tag */
    OFPAT_SET_QUEUE = 21, /* Set queue id when outputting to a port */
    OFPAT_GROUP = 22, /* Apply group. */
    OFPAT_SET_NW_TTL = 23, /* IP TTL. */
    OFPAT_DEC_NW_TTL = 24, /* Decrement IP TTL. */
    OFPAT_SET_FIELD = 25, /* Set a header field using OXM TLV format. */
    OFPAT_PUSH_PBB = 26, /* Push a new PBB service tag (I-TAG) */
    OFPAT_POP_PBB = 27, /* Pop the outer PBB service tag (I-TAG) */
    OFPAT_EXPERIMENTER = 0xffff
};

/*Output, group, and set-queue actions are described in Section 5.12, tag push/pop actions are described
in Table 6, and Set-Field actions are described from their OXM types in Table 11. An action denition
contains the action type, length, and any associated data:*/

/* Action header that is common to all actions. The length includes the
* header and any padding used to make the action 64-bit aligned.
* NB: The length of an action *must* always be a multiple of eight. */
struct ofp_action_header {
    uint16_t type; /* One of OFPAT_*. */
    uint16_t len; /* Length of action, including this
    header. This is the length of action,
    including any padding to make it
    64-bit aligned. */
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_action_header) == 8);

/*An Output action uses the following structure and elds:*/
/* Action structure for OFPAT_OUTPUT, which sends packets out 'port'.
* When the 'port' is the OFPP_CONTROLLER, 'max_len' indicates the max
* number of bytes to send. A 'max_len' of zero means no bytes of the
* packet should be sent. A 'max_len' of OFPCML_NO_BUFFER means that
* the packet is not buffered and the complete packet is to be sent to
* the controller. */
struct ofp_action_output {
    uint16_t type; /* OFPAT_OUTPUT. */
    uint16_t len; /* Length is 16. */
    uint32_t port; /* Output port. */
    uint16_t max_len; /* Max length to send to controller. */
    uint8_t pad[6]; /* Pad to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_action_output) == 16);

/*The port species the port through which the packet should be sent. The max_len indicates the
maximum amount of data from a packet that should be sent when the port is OFPP_CONTROLLER. If
max_len is zero, the switch must send zero bytes of the packet. A max_len of OFPCML_NO_BUFFER
means that the complete packet should be sent, and it should not be buered.*/

enum ofp_controller_max_len {
    OFPCML_MAX = 0xffe5, /* maximum max_len value which can be used
    to request a specific byte length. */
    OFPCML_NO_BUFFER = 0xffff /* indicates that no buffering should be
    applied and the whole packet is to be
    sent to the controller. */
};

/*A Group action uses the following structure and elds:*/
/* Action structure for OFPAT_GROUP. */
struct ofp_action_group {
    uint16_t type; /* OFPAT_GROUP. */
    uint16_t len; /* Length is 8. */
    uint32_t group_id; /* Group identifier. */
};
OFP_ASSERT(sizeof(struct ofp_action_group) == 8);

/*The group_id indicates the group used to process this packet. The set of buckets to apply depends on
the group type.
The Set-Queue action sets the queue id that will be used to map a 
ow entry to an already-congured
queue on a port, regardless of the ToS and VLAN PCP bits. The packet should not change as a result
of a Set-Queue action. If the switch needs to set the ToS/PCP bits for internal handling, the original
values should be restored before sending the packet out.
A switch may support only queues that are tied to specic PCP/ToS bits. In that case, we cannot
map an arbitrary 
ow entry to a specic queue, therefore the Set-Queue action is not supported. The
user can still use these queues and map 
ow entries to them by setting the relevant elds (ToS, VLAN
PCP).
A Set Queue action uses the following structure and elds:*/
/* OFPAT_SET_QUEUE action struct: send packets to given queue on port. */
struct ofp_action_set_queue {
    uint16_t type; /* OFPAT_SET_QUEUE. */
    uint16_t len; /* Len is 8. */
    uint32_t queue_id; /* Queue id for the packets. */
};
OFP_ASSERT(sizeof(struct ofp_action_set_queue) == 8);

/*A Set MPLS TTL action uses the following structure and elds:*/
/* Action structure for OFPAT_SET_MPLS_TTL. */
struct ofp_action_mpls_ttl {
    uint16_t type; /* OFPAT_SET_MPLS_TTL. */
    uint16_t len; /* Length is 8. */
    uint8_t mpls_ttl; /* MPLS TTL */
    uint8_t pad[3];
};

OFP_ASSERT(sizeof(struct ofp_action_mpls_ttl) == 8);

/*The mpls_ttl eld is the MPLS TTL to set.
A Decrement MPLS TTL action takes no arguments and consists only of a generic ofp_action_header.
The action decrements the MPLS TTL.
A Set IPv4 TTL action uses the following structure and elds:*/

/* Action structure for OFPAT_SET_NW_TTL. */
struct ofp_action_nw_ttl {
    uint16_t type; /* OFPAT_SET_NW_TTL. */
    uint16_t len; /* Length is 8. */
    uint8_t nw_ttl; /* IP TTL */
    uint8_t pad[3];
};
OFP_ASSERT(sizeof(struct ofp_action_nw_ttl) == 8);

/*The nw_ttl eld is the TTL address to set in the IP header.
An Decrement IPv4 TTL action takes no arguments and consists only of a generic ofp_action_header.
This action decrement the TTL in the IP header if one is present.
A Copy TTL outwards action takes no arguments and consists only of a generic ofp_action_header.
The action copies the TTL from the next-to-outermost header with TTL to the outermost header with
TTL.
A Copy TTL inwards action takes no arguments and consists only of a generic ofp_action_header.
The action copies the TTL from the outermost header with TTL to the next-to-outermost header with
TTL.
A Push VLAN header action, Push MPLS header and Push PBB header action use the following
structure and elds:*/

/* Action structure for OFPAT_PUSH_VLAN/MPLS/PBB. */
struct ofp_action_push {
    uint16_t type; /* OFPAT_PUSH_VLAN/MPLS/PBB. */
    uint16_t len; /* Length is 8. */
    uint16_t ethertype; /* Ethertype */
    uint8_t pad[2];
};
OFP_ASSERT(sizeof(struct ofp_action_push) == 8);

/*The ethertype indicates the Ethertype of the new tag. It is used when pushing a new VLAN tag, new
MPLS header or PBB service header.
The Push PBB header action logically pushes a new PBB service instance header onto the packet (ITAG
TCI), and copy the original Ethernet addresses of the packet into the customer addresses (C-DA
and C-SA) of the tag. The customer addresses of the I-TAG are in the location of the original Ethernet
addresses of the encapsulated packet, therefore this operations can be seen as adding both the backbone
MAC-in-MAC header and the I-SID eld to the front of the packet. The Push PBB header action
does not add a backbone VLAN header (B-TAG) to the packet, it can be added via the Push VLAN
header action. After this operation, regular set-eld actions can be used to modify the outer Ethernet
addresses (B-DA and B-SA).
A Pop VLAN header action takes no arguments and consists only of a generic ofp_action_header.
The action pops the outermost VLAN tag from the packet.
A Pop PBB header action takes no arguments and consists only of a generic ofp_action_header. The
action logically pops the outer-most PBB service instance header from the packet (I-TAG TCI) and
copy the customer addresses (C-DA and C-SA) in the Ethernet addresses of the packet. This operation
59 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
can be seen as removing the backbone MAC-in-MAC header and the I-SID eld from the front of the
packet. The Pop PBB header action does not remove the backbone VLAN header (B-TAG) from the
packet, it should be removed prior to this operation via the Pop VLAN header action.
A Pop MPLS header action uses the following structure and elds:*/

/* Action structure for OFPAT_POP_MPLS. */
struct ofp_action_pop_mpls {
    uint16_t type; /* OFPAT_POP_MPLS. */
    uint16_t len; /* Length is 8. */
    uint16_t ethertype; /* Ethertype */
    uint8_t pad[2];
};
OFP_ASSERT(sizeof(struct ofp_action_pop_mpls) == 8);

/*The ethertype indicates the Ethertype of the payload.
Set Field actions uses the following structure and elds:*/

/* Action structure for OFPAT_SET_FIELD. */
struct ofp_action_set_field {
    uint16_t type; /* OFPAT_SET_FIELD. */
    uint16_t len; /* Length is padded to 64 bits. */
    /* Followed by:
    * - Exactly oxm_len bytes containing a single OXM TLV, then
    * - Exactly ((oxm_len + 4) + 7)/8*8 - (oxm_len + 4) (between 0 and 7)
    * bytes of all-zero bytes
    */
    uint8_t field[4]; /* OXM TLV - Make compiler happy */
};
OFP_ASSERT(sizeof(struct ofp_action_set_field) == 8);

/*The field contains a header eld described using a single OXM TLV structure (see A.2.3). Set-Field
actions are dened by oxm_type, the type of the OXM TLV, and modify the corresponding header eld in
the packet with the value of oxm_value, the payload of the OXM TLV. The value of oxm_hasmask must
be zero and no oxm_mask is included. The match of the 
ow entry must contain the OXM prerequisite
curresponding to the eld to be set (see A.2.3.6), otherwise an error must be generated (see 6.4).
The type of a set-eld action can be any valid OXM header type, the list of possible OXM types
are described in Section A.2.3.7 and Table 11. Set-Field actions for OXM types OFPXMT_OFB_IN_PORT,
OXM_OF_IN_PHY_PORT and OFPXMT_OFB_METADATA are not supported, because those are not header elds.
The Set-Field action overwrite the header eld specied by the OXM type, and perform the necessary
CRC recalculation based on the header eld. The OXM elds refers to the outermost-possible occurence
in the header, unless the eld type explictely species otherwise, and therefore in general the set-eld
actions applies to the outermost-possible header (e.g. a \Set VLAN ID" set-eld action always sets the
ID of the outermost VLAN tag).
An Experimenter action uses the following structure and elds:*/

/* Action header for OFPAT_EXPERIMENTER.
* The rest of the body is experimenter-defined. */
struct ofp_action_experimenter_header {
    uint16_t type; /* OFPAT_EXPERIMENTER. */
    uint16_t len; /* Length is a multiple of 8. */
    uint32_t experimenter; /* Experimenter ID which takes the same
    form as in struct
    ofp_experimenter_header. */
};
OFP_ASSERT(sizeof(struct ofp_action_experimenter_header) == 8);

/*The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).*/

