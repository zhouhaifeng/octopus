/*A.3.9 Role Request Message
When the controller wants to change its role, it uses the OFPT_ROLE_REQUEST message with the following
structure:*/
/* Role request and reply message. */
struct ofp_role_request {
    struct ofp_header header; /* Type OFPT_ROLE_REQUEST/OFPT_ROLE_REPLY. */
    uint32_t role; /* One of NX_ROLE_*. */
    uint8_t pad[4]; /* Align to 64 bits. */
    uint64_t generation_id; /* Master Election Generation Id */
};
OFP_ASSERT(sizeof(struct ofp_role_request) == 24);
The eld role is the new role that the controller wants to assume, and can have the following values:
/* Controller roles. */
enum ofp_controller_role {
    OFPCR_ROLE_NOCHANGE = 0, /* Don't change current role. */
    OFPCR_ROLE_EQUAL = 1, /* Default role, full access. */
    OFPCR_ROLE_MASTER = 2, /* Full access, at most one master. */
    OFPCR_ROLE_SLAVE = 3, /* Read-only access. */
};
/*If the role value is OFPCR_ROLE_MASTER, all other controllers which role was OFPCR_ROLE_MASTER are
changed to OFPCR_ROLE_SLAVE. If the role value is OFPCR_ROLE_NOCHANGE, the current role of the controller
is not changed ; this enable a controller to query its current role without changing it.
Upon receipt of a OFPT_ROLE_REQUEST message, the switch must return a OFPT_ROLE_REPLY message.
The structure of this message is exactly the same as the OFPT_ROLE_REQUEST message, and the eld
role is the current role of the controller.
Additionally, if the role value in the message is OFPCR_ROLE_MASTER or OFPCR_ROLE_SLAVE, the switch
must validate generation_id to check for stale messages. If the validation fails, the switch must
discard the role request and return an error message with type OFPET_ROLE_REQUEST_FAILED and code
OFPRRFC_STALE.*/
/*A.3.10 Set Asynchronous Conguration Message
The controller is able to set and query the asynchronous messages that it wants to receive (other than
error messages) on a given OpenFlow channel with the OFPT_SET_ASYNC and OFPT_GET_ASYNC_REQUEST
messages, respectively. The switch responds to a OFPT_GET_ASYNC_REQUEST message with an
OFPT_GET_ASYNC_REPLY message; it does not reply to a request to set the conguration.
There is no body for OFPT_GET_ASYNC_REQUEST beyond the OpenFlow header. The OFPT_SET_ASYNC
and OFPT_GET_ASYNC_REPLY messages have the following format:*/
/* Asynchronous message configuration. */
struct ofp_async_config {
    struct ofp_header header; /* OFPT_GET_ASYNC_REPLY or OFPT_SET_ASYNC. */
    uint32_t packet_in_mask[2]; /* Bitmasks of OFPR_* values. */
    uint32_t port_status_mask[2]; /* Bitmasks of OFPPR_* values. */
    uint32_t flow_removed_mask[2];/* Bitmasks of OFPRR_* values. */
};
OFP_ASSERT(sizeof(struct ofp_async_config) == 32);
/*struct ofp_async_config contains three 2-element arrays. Each array controls whether the controller
receives asynchronous messages with a specic enum ofp_type. Within each array, element 0 species
messages of interest when the controller has a OFPCR_ROLE_EQUAL or OFPCR_ROLE_MASTER role; element
1, when the controller has a OFPCR_ROLE_SLAVE role. Each array element is a bit-mask in which a
0-bit disables receiving a message sent with the reason code corresponding to the bit index and a
1-bit enables receiving it. For example, the bit with value 22 = 4 in port_status_mask[1] determines
whether the controller will receive OFPT_PORT_STATUS messages with reason OFPPR_MODIFY (value 2)
when the controller has role OFPCR_ROLE_SLAVE.
OFPT_SET_ASYNC sets whether a controller should receive a given asynchronous message that is generated
by the switch. Other OpenFlow features control whether a given message is generated; for example,
the OFPFF_SEND_FLOW_REM 
ag controls whether the switch generates OFPT_FLOW_REMOVED a message
when a 
ow entry is removed.
A switch conguration, for example using the OpenFlow Conguration Protocol, may set the initial
conguration of asynchronous messages when an OpenFlow connection is established. In the absence
of such switch conguration, the initial conguration shall be:
. In the \master" or \equal" role, enable all OFPT_PACKET_IN messages, except those with reason
OFPR_INVALID_TTL, and enable all OFPT_PORT_STATUS and OFPT_FLOW_REMOVED messages.
. In the \slave" role, enable all OFPT_PORT_STATUS messages and disable all OFPT_PACKET_IN and
OFPT_FLOW_REMOVED messages.
The conguration set with OFPT_SET_ASYNC is specic to a particular OpenFlow channel. It does not
aect any other OpenFlow channel, whether currently established or to be established in the future.
The conguration set with OFPT_SET_ASYNC does not lter or otherwise aect error messages.
*/
