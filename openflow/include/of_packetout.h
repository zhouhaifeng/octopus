
/*A.3.7 Packet-Out Message
When the controller wishes to send a packet out through the datapath, it uses the OFPT_PACKET_OUT
message:*/
/* Send packet (controller -> datapath). */
struct ofp_packet_out {
    struct ofp_header header;
    uint32_t buffer_id; /* ID assigned by datapath (OFP_NO_BUFFER
    if none). */
    uint32_t in_port; /* Packet's input port or OFPP_CONTROLLER. */
    uint16_t actions_len; /* Size of action array in bytes. */
    uint8_t pad[6];
    struct ofp_action_header actions[0]; /* Action list. */
    /* uint8_t data[0]; */ /* Packet data. The length is inferred
    from the length field in the header.
    (Only meaningful if buffer_id == -1.) */
};
OFP_ASSERT(sizeof(struct ofp_packet_out) == 24);

/*The buffer_id is the same given in the ofp_packet_in message. If the buffer_id is OFP_NO_BUFFER,
then the packet data is included in the data array. OFP_NO_BUFFER is 0x.
The in_port eld is the ingress port that must be associated with the packet for OpenFlow processing.
It must be set to either a valid standard switch port or OFPP_CONTROLLER.
The action eld is an action list dening how the packet should be processed by the switch. It may include
packet modication, group processing and an output port. The action list of an OFPT_PACKET_OUT
message can also specify the OFPP_TABLE reserved port as an output action to process the packet through
the existing 
ow entries, starting at the rst 
ow table. If OFPP_TABLE is specied, the in_port eld is
used as the ingress port in the 
ow table lookup.
In some cases, packets sent to OFPP_TABLE may be forwarded back to the controller as the result of a 
ow
entry action or table miss. Detecting and taking action for such controller-to-switch loops is outside the
scope of this specication. In general, OpenFlow messages are not guaranteed to be processed in order,
therefore if a OFPT_PACKET_OUT message using OFPP_TABLE depends on a 
ow that was recently sent to
the switch (with a OFPT_FLOW_MOD message), a OFPT_BARRIER_REQUEST message may be required prior
to the OFPT_PACKET_OUT message to make sure the 
ow entry was committed to the 
ow table prior to
execution of OFPP_TABLE.
89 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
A.3.8 Barrier Message
When the controller wants to ensure message dependencies have been met or wants to receive notications
for completed operations, it may use an OFPT_BARRIER_REQUEST message. This message has no
body. Upon receipt, the switch must nish processing all previously-received messages, including sending
corresponding reply or error messages, before executing any messages beyond the Barrier Request.
When such processing is complete, the switch must send an OFPT_BARRIER_REPLY message with the xid
of the original request.*/
