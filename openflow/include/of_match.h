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
/*A.2.3 Flow Match Structures*/

/* match header: Fields to match against flows */
struct ofp_match {
    uint16_t type; /* One of OFPMT_* */
    uint16_t length; /* Length of ofp_match (excluding padding) */
    /* Followed by:
    * - Exactly (length - 4) (possibly 0) bytes containing OXM TLVs, then
    * - Exactly ((length + 7)/8*8 - length) (between 0 and 7) bytes of
    * all-zero bytes
    * In summary, ofp_match is padded as needed, to make its overall size
    * a multiple of 8, to preserve alignement in structures using it.
    */
    uint8_t oxm_fields[4]; /* OXMs start here - Make compiler happy */
};
OFP_ASSERT(sizeof(struct ofp_match) == 8);

/*The type eld is set to OFPMT_OXM and length eld is set to the actual length of ofp_match structure
including all match elds. The payload of the OpenFlow match is a set of OXM Flow match elds.*/

/* The match type indicates the match structure (set of fields that compose the
* match) in use. The match type is placed in the type field at the beginning
* of all match structures. The "OpenFlow Extensible Match" type corresponds
* to OXM TLV format described below and must be supported by all OpenFlow
* switches. Extensions that define other match types may be published on the
* ONF wiki. Support for extensions is optional.
*/
enum ofp_match_type {
    OFPMT_STANDARD = 0, /* Deprecated. */
    OFPMT_OXM = 1, /* OpenFlow Extensible Match */
};

/*The only valid match type in this specication is OFPMT_OXM, the OpenFlow 1.1 match type
OFPMT_STANDARD is deprecated. If an alternate match type is used, the match elds and payload may
be set dierently, but this is outside the scope of this specication.
A.2.3.2 Flow Match Field Structures
The 
ow match elds are described using the OpenFlow Extensible Match (OXM) format, which is a
compact type-length-value (TLV) format. Each OXM TLV is 5 to 259 (inclusive) bytes long. OXM
TLVs are not aligned on or padded to any multibyte boundary. The rst 4 bytes of an OXM TLV are
its header, followed by the entry's body.
45 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
31 16 15 9 8 7 0
oxm class oxm eld
HM
oxm length
Figure~4: OXM TLV header layout.
An OXM TLV's header is interpreted as a 32-bit word in network byte order (see gure 4).
The OXM TLV's header elds are dened in Table 9
Name Width Usage
oxm type
oxm_class 16 Match class: member class or reserved class
oxm_field 7 Match eld within the class
oxm_hasmask 1 Set if OXM include a bitmask in payload
oxm_length 8 Length of OXM payload
Table~9: OXM TLV header elds.
The oxm_class is a OXM match class that contains related match types, and is described in section
A.2.3.3. oxm_field is an class-specic value, identifying one of the match types within the match
class. The combination of oxm_class and oxm_field (the most-signicant 23 bits of the header) are
collectively oxm_type. The oxm_type normally designates a protocol header eld, such as the Ethernet
type, but it can also refer to packet metadata, such as the switch port on which a packet arrived.
oxm_hasmask denes if the OXM TLV contains a bitmask, more details is explained in section A.2.3.5.
oxm_length is a positive integer describing the length of the OXM TLV payload in bytes. The length
of the OXM TLV, including the header, is exactly 4 + oxm_length bytes.
For a given oxm_class, oxm_field, and oxm_hasmask value, oxm_length is a constant. It is included
only to allow software to minimally parse OXM TLVs of unknown types. (Similarly, for a
given oxm_class, oxm_field, and oxm_length, oxm_hasmask is a constant.)
A.2.3.3 OXM classes
The match types are structured using OXM match classes. The OpenFlow specication distinguishes
two types of OXM match classes, ONF member classes and ONF reserved classes, dierentiated by their
high order bit. Classes with the high order bit set to 1 are ONF reserved classes, they are used for the
OpenFlow specication itself. Classes with the high order bit set to zero are ONF member classes, they
are allocated by the ONF on an as needed basis, they uniquely identify an ONF member and can be
used arbitrarily by that member. Support for ONF member classes is optional.
The following OXM classes are dened:*/

/* OXM Class IDs.
* The high order bit differentiate reserved classes from member classes.
* Classes 0x0000 to 0x7FFF are member classes, allocated by ONF.
* Classes 0x8000 to 0xFFFE are reserved classes, reserved for standardisation.
*/

enum ofp_oxm_class {
    OFPXMC_NXM_0 = 0x0000, /* Backward compatibility with NXM */
    46 . 2012; The Open Networking Foundation
    OpenFlow Switch Specication Version 1.3.1
    OFPXMC_NXM_1 = 0x0001, /* Backward compatibility with NXM */
    OFPXMC_OPENFLOW_BASIC = 0x8000, /* Basic class for OpenFlow */
    OFPXMC_EXPERIMENTER = 0xFFFF, /* Experimenter class */
};

/*The class OFPXMC_OPENFLOW_BASIC contains the basic set of OpenFlow match elds (see A.2.3.7). The
optional class OFPXMC_EXPERIMENTER is used for experimenter matches (see A.2.3.8). Other ONF reserved
classes are reserved for future uses such as modularisation of the specication. The rst two
ONF member classes OFPXMC_NXM_0 and OFPXMC_NXM_1 are reserved for backward compatibility with
the Nicira Extensible Match (NXM) specication.
A.2.3.4 Flow Matching
A zero-length OpenFlow match (one with no OXM TLVs) matches every packet. Match elds that
should be wildcarded are omitted from the OpenFlow match.
An OXM TLV places a constraint on the packets matched by the OpenFlow match:
. If oxm_hasmask is 0, the OXM TLV's body contains a value for the eld, called oxm_value. The
OXM TLV match matches only packets in which the corresponding eld equals oxm_value.
. If oxm_hasmask is 1, then the oxm_entry's body contains a value for the eld (oxm_value), followed
by a bitmask of the same length as the value, called oxm_mask. The OXM TLV match matches
only packets in which the corresponding eld equals oxm_value for the bits dened by oxm_mask.
When there are multiple OXM TLVs, all of the constraints must be met: the packet elds must match
all OXM TLVs part of the OpenFlow match. The elds for which OXM TLVs that are not present are
wildcarded to ANY, omitted OXM TLVs are eectively fully masked to zero.
A.2.3.5 Flow Match Field Masking
When oxm_hasmask is 1, the OXM TLV contains a bitmask and its length is eectively doubled, so
oxm_length is always even. The bitmask follows the eld value and is encoded in the same way. The
masks are dened such that a 0 in a given bit position indicates a \don't care" match for the same bit
in the corresponding eld, whereas a 1 means match the bit exactly.
An all-zero-bits oxm_mask is equivalent to omitting the OXM TLV entirely. An all-one-bits oxm_mask
is equivalent to specifying 0 for oxm_hasmask and omitting oxm_mask.
Some oxm_types may not support masked wildcards, that is, oxm_hasmask must always be 0 when
these elds are specied. For example, the eld that identies the ingress port on which a packet was
received may not be masked.
Some oxm_types that do support masked wildcards may only support certain oxm_mask patterns. For
example, some elds that have IPv4 address values may be restricted to CIDR masks (subnet masks).
These restrictions are detailed in specications for individual elds. A switch may accept an
oxm_hasmask or oxm_mask value that the specication disallows, but only if the switch correctly implements
support for that oxm_hasmask or oxm_mask value. A switch must reject an attempt to set up a

ow entry that contains a oxm_hasmask or oxm_mask value that it does not support (see 6.4).
47 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
A.2.3.6 Flow Match Field Prerequisite
The presence of an OXM TLV with a given oxm_type may be restricted based on the presence or values
of other OXM TLVs. In general, matching header elds of a protocol can only be done if the OpenFlow
match explitly matches the corresponding protocol.
For example:
. An OXM TLV for oxm_type=OXM OF IPV4 SRC is allowed only if it is preceded by another entry
with oxm_type=OXM_OF_ETH_TYPE, oxm_hasmask=0, and oxm_value=0x0800. That is, matching
on the IPv4 source address is allowed only if the Ethernet type is explicitly set to IPv4.
. An OXM TLV for oxm_type=OXM OF TCP SRC is allowed only if it is preceded by an entry with
oxm_type=OXM OF ETH TYPE, oxm_hasmask=0, oxm_value=0x0800 or 0x86dd, and another
with oxm_type=OXM OF IP PROTO, oxm_hasmask=0, oxm_value=6, in that order. That is,
matching on the TCP source port is allowed only if the Ethernet type is IP and the IP protocol
is TCP.
. An OXM TLV for oxm_type=OXM OF MPLS LABEL is allowed only if it is preceded by an
entry with oxm_type=OXM OF ETH TYPE, oxm_hasmask=0, oxm_value=0x8847 or 0x8848.
. An OXM TLV for oxm_type=OXM OF VLAN PCP is allowed only if it is preceded by an entry
with oxm_type=OXM OF VLAN VID, oxm_value!=OFPVID NONE.
These restrictions are noted in specications for individual elds (see A.2.3.7). A switch may implement
relaxed versions of these restrictions. For example, a switch may accept no prerequisite at all. A switch
must reject an attempt to set up a 
ow entry that violates its restrictions (see 6.4), and must deal with
inconsistent matches created by the lack of prerequisite (for example matching both a TCP source port
and a UDP destination port).
New match elds dened by members (in member classes or as experimenter elds) may provide alternate
prerequisites to already specied match elds. For example, this could be used to reuse existing IP
match elds over an alternate link technology (such as PPP) by substituting the ETH_TYPE prerequisite
as needed (for PPP, that could be an hypothetical PPP_PROTOCOL eld).
An OXM TLV that has prerequisite restrictions must appear after the OXM TLVs for its prerequisites.
Ordering of OXM TLVs within an OpenFlow match is not otherwise constrained.
Any given oxm_type may appear in an OpenFlow match at most once, otherwise the switch must
generate an error (see 6.4). A switch may implement a relaxed version of this rule and may allow
in some cases a oxm_type to appear multiple time in an OpenFlow match, however the behaviour of
matching is then implementation-dened.
If a 
ow table implements a specic OXM TLV, this 
ow table must accept valid matches containing
the prerequisites of this OXM TLV, even if the 
ow table does not support matching all possible values
for the match elds specied by those prerequisites. For example, if a 
ow table matches the IPv4
source address, this 
ow table must accept matching the Ethertype exactly to IPv4, however this 
ow
table does not need to support matching Ethertype to any other value.
A.2.3.7 Flow Match Fields
The specication denes a default set of match elds with oxm_class=OFPXMC_OPENFLOW_BASIC which
can have the following values:*/

/* OXM Flow match field types for OpenFlow basic class. */
enum oxm_ofb_match_fields {
    OFPXMT_OFB_IN_PORT = 0, /* Switch input port. */
    OFPXMT_OFB_IN_PHY_PORT = 1, /* Switch physical input port. */
    OFPXMT_OFB_METADATA = 2, /* Metadata passed between tables. */
    OFPXMT_OFB_ETH_DST = 3, /* Ethernet destination address. */
    OFPXMT_OFB_ETH_SRC = 4, /* Ethernet source address. */
    OFPXMT_OFB_ETH_TYPE = 5, /* Ethernet frame type. */
    OFPXMT_OFB_VLAN_VID = 6, /* VLAN id. */
    OFPXMT_OFB_VLAN_PCP = 7, /* VLAN priority. */
    OFPXMT_OFB_IP_DSCP = 8, /* IP DSCP (6 bits in ToS field). */
    OFPXMT_OFB_IP_ECN = 9, /* IP ECN (2 bits in ToS field). */
    OFPXMT_OFB_IP_PROTO = 10, /* IP protocol. */
    OFPXMT_OFB_IPV4_SRC = 11, /* IPv4 source address. */
    OFPXMT_OFB_IPV4_DST = 12, /* IPv4 destination address. */
    OFPXMT_OFB_TCP_SRC = 13, /* TCP source port. */
    OFPXMT_OFB_TCP_DST = 14, /* TCP destination port. */
    OFPXMT_OFB_UDP_SRC = 15, /* UDP source port. */
    OFPXMT_OFB_UDP_DST = 16, /* UDP destination port. */
    OFPXMT_OFB_SCTP_SRC = 17, /* SCTP source port. */
    OFPXMT_OFB_SCTP_DST = 18, /* SCTP destination port. */
    OFPXMT_OFB_ICMPV4_TYPE = 19, /* ICMP type. */
    OFPXMT_OFB_ICMPV4_CODE = 20, /* ICMP code. */
    OFPXMT_OFB_ARP_OP = 21, /* ARP opcode. */
    OFPXMT_OFB_ARP_SPA = 22, /* ARP source IPv4 address. */
    OFPXMT_OFB_ARP_TPA = 23, /* ARP target IPv4 address. */
    OFPXMT_OFB_ARP_SHA = 24, /* ARP source hardware address. */
    OFPXMT_OFB_ARP_THA = 25, /* ARP target hardware address. */
    OFPXMT_OFB_IPV6_SRC = 26, /* IPv6 source address. */
    OFPXMT_OFB_IPV6_DST = 27, /* IPv6 destination address. */
    OFPXMT_OFB_IPV6_FLABEL = 28, /* IPv6 Flow Label */
    OFPXMT_OFB_ICMPV6_TYPE = 29, /* ICMPv6 type. */
    OFPXMT_OFB_ICMPV6_CODE = 30, /* ICMPv6 code. */
    OFPXMT_OFB_IPV6_ND_TARGET = 31, /* Target address for ND. */
    OFPXMT_OFB_IPV6_ND_SLL = 32, /* Source link-layer for ND. */
    OFPXMT_OFB_IPV6_ND_TLL = 33, /* Target link-layer for ND. */
    OFPXMT_OFB_MPLS_LABEL = 34, /* MPLS label. */
    OFPXMT_OFB_MPLS_TC = 35, /* MPLS TC. */
    OFPXMT_OFP_MPLS_BOS = 36, /* MPLS BoS bit. */
    OFPXMT_OFB_PBB_ISID = 37, /* PBB I-SID. */
    OFPXMT_OFB_TUNNEL_ID = 38, /* Logical Port Metadata. */
    OFPXMT_OFB_IPV6_EXTHDR = 39, /* IPv6 Extension Header pseudo-field */
};

/*A switch must support the required match elds listed in Table 10 in its pipeline. Each required match
eld must be supported in at least one 
ow table of the switch : that 
ow table must enable matching
that eld and the match eld prerequisites must be met in that table (see A.2.3.6). The required elds
don't need to be implemented in all 
ow tables, and don't need to be implemented in the same 
ow
table. Flow tables can support non-required and experimenter match elds. The controller can query
the switch about which match elds are supported in each 
ow table (see A.3.5.5).
Field Description
OXM_OF_IN_PORT Required Ingress port. This may be a physical or switch-dened logical port.
OXM_OF_ETH_DST Required Ethernet destination address. Can use arbitrary bitmask
OXM_OF_ETH_SRC Required Ethernet source address. Can use arbitrary bitmask
OXM_OF_ETH_TYPE Required Ethernet type of the OpenFlow packet payload, after VLAN tags.
OXM_OF_IP_PROTO Required IPv4 or IPv6 protocol number
OXM_OF_IPV4_SRC Required IPv4 source address. Can use subnet mask or arbitrary bitmask
OXM_OF_IPV4_DST Required IPv4 destination address. Can use subnet mask or arbitrary bitmask
OXM_OF_IPV6_SRC Required IPv6 source address. Can use subnet mask or arbitrary bitmask
OXM_OF_IPV6_DST Required IPv6 destination address. Can use subnet mask or arbitrary bitmask
OXM_OF_TCP_SRC Required TCP source port
OXM_OF_TCP_DST Required TCP destination port
OXM_OF_UDP_SRC Required UDP source port
OXM_OF_UDP_DST Required UDP destination port
Table~10: Required match elds.
All match elds have dierent size, prerequisites and masking capability, as specied in Table 11. If
not explictely specied in the eld description, each eld type refer the the outermost occurrence of the
eld in the packet headers.
50 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
Field Bits Mask Pre-requisite Description
OXM_OF_IN_PORT 32 No None Ingress port. Numerical representation of in-
coming port, starting at 1. This may be a phys-
ical or switch-dened logical port.
OXM_OF_IN_PHY_PORT 32 No IN PORT present Physical port. In ofp_packet_in messages, un-
derlying physical port when packet received on
a logical port.
OXM_OF_METADATA 64 Yes None Table metadata. Used to pass information be-
tween tables.
OXM_OF_ETH_DST 48 Yes None Ethernet destination MAC address.
OXM_OF_ETH_SRC 48 Yes None Ethernet source MAC address.
OXM_OF_ETH_TYPE 16 No None Ethernet type of the OpenFlow packet payload,
after VLAN tags.
OXM_OF_VLAN_VID 12+1 Yes None VLAN-ID from 802.1Q header. The CFI bit in-
dicate the presence of a valid VLAN-ID, see be-
low.
OXM_OF_VLAN_PCP 3 No VLAN VID!=NONE VLAN-PCP from 802.1Q header.
OXM_OF_IP_DSCP 6 No ETH TYPE=0x0800 or
ETH TYPE=0x86dd
Di Serv Code Point (DSCP). Part of the IPv4
ToS eld or the IPv6 Trac Class eld.
OXM_OF_IP_ECN 2 No ETH TYPE=0x0800 or
ETH TYPE=0x86dd
ECN bits of the IP header. Part of the IPv4
ToS eld or the IPv6 Trac Class eld.
OXM_OF_IP_PROTO 8 No ETH TYPE=0x0800 or
ETH TYPE=0x86dd
IPv4 or IPv6 protocol number.
OXM_OF_IPV4_SRC 32 Yes ETH TYPE=0x0800 IPv4 source address. Can use subnet mask or
arbitrary bitmask
OXM_OF_IPV4_DST 32 Yes ETH TYPE=0x0800 IPv4 destination address. Can use subnet mask
or arbitrary bitmask
OXM_OF_TCP_SRC 16 No IP PROTO=6 TCP source port
OXM_OF_TCP_DST 16 No IP PROTO=6 TCP destination port
OXM_OF_UDP_SRC 16 No IP PROTO=17 UDP source port
OXM_OF_UDP_DST 16 No IP PROTO=17 UDP destination port
OXM_OF_SCTP_SRC 16 No IP PROTO=132 SCTP source port
OXM_OF_SCTP_DST 16 No IP PROTO=132 SCTP destination port
OXM_OF_ICMPV4_TYPE 8 No IP PROTO=1 ICMP type
OXM_OF_ICMPV4_CODE 8 No IP PROTO=1 ICMP code
OXM_OF_ARP_OP 16 No ETH TYPE=0x0806 ARP opcode
OXM_OF_ARP_SPA 32 Yes ETH TYPE=0x0806 Source IPv4 address in the ARP payload. Can
use subnet mask or arbitrary bitmask
OXM_OF_ARP_TPA 32 Yes ETH TYPE=0x0806 Target IPv4 address in the ARP payload. Can
use subnet mask or arbitrary bitmask
OXM_OF_ARP_SHA 48 Yes ETH TYPE=0x0806 Source Ethernet address in the ARP payload.
OXM_OF_ARP_THA 48 Yes ETH TYPE=0x0806 Target Ethernet address in the ARP payload.
OXM_OF_IPV6_SRC 128 Yes ETH TYPE=0x86dd IPv6 source address. Can use subnet mask or
arbitrary bitmask
OXM_OF_IPV6_DST 128 Yes ETH TYPE=0x86dd IPv6 destination address. Can use subnet mask
or arbitrary bitmask
OXM_OF_IPV6_FLABEL 20 Yes ETH TYPE=0x86dd IPv6 
ow label.
OXM_OF_ICMPV6_TYPE 8 No IP PROTO=58 ICMPv6 type
OXM_OF_ICMPV6_CODE 8 No IP PROTO=58 ICMPv6 code
OXM_OF_IPV6_ND_TARGET 128 No ICMPV6 TYPE=135 or
ICMPV6 TYPE=136
The target address in an IPv6 Neighbor Discov-
ery message.
Table 11 { Continued on next page
51 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
Table 11 { concluded from previous page
Field Bits Mask Pre-requisite Description
OXM_OF_IPV6_ND_SLL 48 No ICMPV6 TYPE=135 The source link-layer address option in an IPv6
Neighbor Discovery message.
OXM_OF_IPV6_ND_TLL 48 No ICMPV6 TYPE=136 The target link-layer address option in an IPv6
Neighbor Discovery message.
OXM_OF_MPLS_LABEL 20 No ETH TYPE=0x8847 or
ETH TYPE=0x8848
The LABEL in the rst MPLS shim header.
OXM_OF_MPLS_TC 3 No ETH TYPE=0x8847 or
ETH TYPE=0x8848
The TC in the rst MPLS shim header.
OXM_OF_MPLS_BOS 1 No ETH TYPE=0x8847 or
ETH TYPE=0x8848
The BoS bit in the rst MPLS shim header.
OXM_OF_PBB_ISID 24 Yes ETH TYPE=0x88E7 The I-SID in the rst PBB service instance tag.
OXM_OF_TUNNEL_ID 64 Yes None Metadata associated with a logical port.
OXM_OF_IPV6_EXTHDR 9 Yes ETH TYPE=0x86dd IPv6 Extension Header pseudo-eld.
Table~11: Match elds details.
The ingress port OXM_OF_IN_PORT is a valid standard OpenFlow port, either a physical, a logical
port, the OFPP_LOCAL reserved port or the OFPP_CONTROLLER reserved port. The physical port
OXM_OF_IN_PHY_PORT is used in Packet-in messages to identify a physical port underneath a logical
port (see A.4.1).
The metadata eld OXM_OF_METADATA is used to pass information between lookups across multiple
tables. This value can be arbitrarily masked.
The Tunnel ID eld OXM_OF_TUNNEL_ID carries optional metadata associated with a logical port. The
mapping of this metadata is dened by the logical port implementation. If the logical port does not
provide such data or if the packet was received on a physical port, its value is zero. For example, for
a packet received via GRE tunnel including a (32-bit) key, the key is stored in the low 32-bits and the
high bits are zeroed. For a MPLS logical port, the low 20 bits represent the MPLS Label. For a VxLAN
logical port, the low 24 bits represent the VNI.
Omitting the OFPXMT_OFB_VLAN_VID eld species that a 
ow entry should match packets regardless
of whether they contain the corresponding tag. Special values are dened below for the VLAN tag
to allow matching of packets with any tag, independent of the tag's value, and to supports matching
packets without a VLAN tag. The special values dened for OFPXMT_OFB_VLAN_VID are:*/

/* The VLAN id is 12-bits, so we can use the entire 16 bits to indicate
* special conditions.
*/
enum ofp_vlan_id {
    OFPVID_PRESENT = 0x1000, /* Bit that indicate that a VLAN id is set */
    OFPVID_NONE = 0x0000, /* No VLAN id was set. */
};

/*The OFPXMT_OFB_VLAN_PCP eld must be rejected when the OFPXMT_OFB_VLAN_VID eld is wildcarded
(not present) or when the value of OFPXMT_OFB_VLAN_VID is set to OFPVID_NONE. Table 12 summarizes
the combinations of wildcard bits and eld values for particular VLAN tag matches.
52 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
OXM eld oxm value oxm mask Matching packets
absent - - Packets with and without a VLAN tag
present OFPVID_NONE absent Only packets without a VLAN tag
present OFPVID_PRESENT OFPVID_PRESENT Only packets with a VLAN tag regardless of its
value
present value j OFPVID_PRESENT absent Only packets with VLAN tag and VID equal
value
Table~12: Match combinations for VLAN tags.
The eld OXM_OF_IPV6_EXTHDR is a pseudo eld that indicates the presence of various IPv6 extension
headers in the packet header. The IPv6 extension header bits are combined together in the elds
OXM_OF_IPV6_EXTHDR, and those bits can have the following values:*/

/* Bit definitions for IPv6 Extension Header pseudo-field. */
enum ofp_ipv6exthdr_flags {
    OFPIEH_NONEXT = 1 << 0, /* "No next header" encountered. */
    OFPIEH_ESP = 1 << 1, /* Encrypted Sec Payload header present. */
    OFPIEH_AUTH = 1 << 2, /* Authentication header present. */
    OFPIEH_DEST = 1 << 3, /* 1 or 2 dest headers present. */
    OFPIEH_FRAG = 1 << 4, /* Fragment header present. */
    OFPIEH_ROUTER = 1 << 5, /* Router header present. */
    OFPIEH_HOP = 1 << 6, /* Hop-by-hop header present. */
    OFPIEH_UNREP = 1 << 7, /* Unexpected repeats encountered. */
    OFPIEH_UNSEQ = 1 << 8, /* Unexpected sequencing encountered. */
};

/*. OFPIEH_HOP is set to 1 if a hop-by-hop IPv6 extension header is present as the rst extension
header in the packet.
. OFPIEH_ROUTER is set to 1 if a router IPv6 extension header is present.
. OFPIEH_FRAG is set to 1 if a fragmentation IPv6 extension header is present.
. OFPIEH_DEST is set to 1 if one or more Destination options IPv6 extension headers are present. It
is normal to have either one or two of these in one IPv6 packet (see RFC 2460).
. OFPIEH_AUTH is set to 1 if an Authentication IPv6 extension header is present.
. OFPIEH_ESP is set to 1 if an Encrypted Security Payload IPv6 extension header is present.
. OFPIEH_NONEXT is set to 1 if a No Next Header IPv6 extension header is present.
. OFPIEH_UNSEQ is set to 1 if IPv6 extension headers were not in the order preferred (but not
required) by RFC 2460.
. OFPIEH_UNREP is set to 1 if more than one of a given IPv6 extension header is unexpectedly
encountered. (Two destination options headers may be expected and would not cause this bit to
be set.)
A.2.3.8 Experimenter Flow Match Fields
Support for experimenter-specic 
ow match elds is optional. Experimenter-specic 
ow match elds
may dened using the oxm_class=OFPXMC_EXPERIMENTER. The rst four bytes of the OXM TLV's body
contains the experimenter identier, which takes the same form as in struct ofp_experimenter. Both
oxm_field and the rest of the OXM TLV is experimenter-dened and does not need to be padded or
aligned.*/

/* Header for OXM experimenter match fields. */
struct ofp_oxm_experimenter_header {
    uint32_t oxm_header; /* oxm_class = OFPXMC_EXPERIMENTER */
    uint32_t experimenter; /* Experimenter ID which takes the same
    form as in struct ofp_experimenter_header. */
};
OFP_ASSERT(sizeof(struct ofp_oxm_experimenter_header) == 8);


