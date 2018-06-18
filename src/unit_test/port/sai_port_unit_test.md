# SAI PORT UNIT TEST

	

Test case number : 1

Title   : Logical Port Type

Purpose : Validates Port type get for all logical ports in the switch

Topology: Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_TYPE get port attribute with parameters port_id, attr_count=1, attribute list

2)Verify return value is SAI_STATUS_SUCCESS

3)Verify s32 value is SAI_PORT_TYPE_LOGICAL

Test case number : 2

Title   : CPU Port Type

Purpose : Validates Port type get for CPU port in the switch

Topology: Standalone unit

Procedure:

1)For attribute id SAI_SWITCH_ATTR_CPU_PORT get switch attribute with parameters switch_id=0, attr_count=1, attribute list

2)Verify return value is SAI_STATUS_SUCCESS

3)Assign oid value of attribute to object_id cpu_port

4)For attribute id SAI_PORT_ATTR_TYPE get port attribute with parameters cpu_port as port_id, attr_count=1, attribute list

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is SAI_PORT_TYPE_CPU

Test case number : 3

Title	:  Port Operational Status

Purpose : Validates Port Operational Status get for all the valid ports in the switch

Topology: Standalone unit

Procedure:

1)Set booldata value as true for attribute id SAI_PORT_ATTR_ADMIN_STATE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_ADMIN_STATE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true

4)For attribute id SAI_PORT_ATTR_OPER_STATUS get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is SAI_PORT_OPER_STATUS_UP

Test for all valid ports in the switch

Test case number : 4

Title	:  Port Speed

Purpose :  Validates if the port is able to get/set a given port speed

Topology:  Standalone unit

Procedure:

1)Set the speed value as u32 value for attribute id SAI_PORT_ATTR_SPEED

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_SPEED get port attribute with port_id , 1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is the speed value given

Test for all speed values 100G , 50G , 40G , 25G ,10G , 1G , 100M


Test case number : 5

Title : Port EEE state

Purpose : Validates if port EEE state can be enabled for all the copper ports in the switch

Topology:  Standalone unit

Procedure:

1)Set booldata value as true for attribute id SAI_PORT_ATTR_EEE_ENABLE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_EEE_ENABLE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true

Test in all valid ports


Test case number : 6

Title : Port EEE IDLE Time

Purpose : Validates if port EEE IDLE time can be set for all the copper ports in the switch

Topology:  Standalone unit

Procedure:

1)Set a EEE IDLE Time value u16 value for attribute id SAI_PORT_ATTR_EEE_IDLE_TIME

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_EEE_IDLE_TIME get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify u16 value is the EEE IDLE Time value given

7)Set a EEE WAKE Time value u16 value for attribute id SAI_PORT_ATTR_EEE_WAKE_TIME

8)Set port attribute for the port method table with port_id , attribute as parameters

9)Verify return value is SAI_STATUS_SUCCESS

10)For attribute id SAI_PORT_ATTR_EEE_WAKE_TIME get port attribute with port_id , attr_count=1 , attribute list as parameters

11)Verify return value is SAI_STATUS_SUCCESS

12)Verify u16 value is equal to the EEE WAKE Time value given

Test in all valid ports


Test case number : 7

Title : Port Location LED

Purpose : Validates if port location LED can be enabled for all the valid ports in the switch

Topology:  Standalone unit

Procedure:

1)Set booldata value as true for attribute id SAI_PORT_ATTR_LOCATION_LED

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_LOCATION_LED get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true

Test in all valid ports


Test case number : 8

Title : Port default VLAN

Purpose : Validates if the port default VLAN can be set to 100

Topology:  Standalone unit

Procedure:

1)Set u16 value as 100 for attribute id SAI_PORT_ATTR_PORT_VLAN_ID

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_PORT_VLAN_ID get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify u16 value is 100



Test case number : 9

Title : Port default VLAN priority

Purpose : Validates if the port default VLAN priority can be set to 1

Topology:  Standalone unit

Procedure:

1)Set u8 value as 1 for attribute id SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify u8 value is 1


Test case number : 10

Title : Port Drop Untagged

Purpose : Validates if the port drop untagged capability can be enabled

Topology:  Standalone unit

Procedure:

1)Set booldata value as true for attribute id SAI_PORT_ATTR_DROP_UNTAGGED

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_DROP_UNTAGGED get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true


Test case number : 11

Title : Port Drop Tagged

Purpose : Validates if the port drop tagged capability can be enabled

Topology:  Standalone unit

Procedure:

1)Set booldata value as true for attribute id SAI_PORT_ATTR_DROP_TAGGED

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_DROP_TAGGED get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true


Test different combinations of true , false for drop Untagged , drop Tagged and
verify setting drop untagged is not affecting drop tagged and vice-versa


Test case number : 12

Title : Port Loopback Mode

Purpose : Validates if a given looback mode gets configured correctly on a given port

Topology:  Standalone unit

Procedure:

1)Set loopback mode value as s32 value for attribute id SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value of the attribute is the loopback mode value given

Test for the loopback mode values
SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY
SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC
SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE

Test in all valid ports


Test case number : 13

Title : Port Auto Negotiation

Purpose : Validates if port autoneg can be enabled for all the valid ports in the switch

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

2)Verify return value is SAI_STATUS_SUCCESS

3)Verify s32 value of the attribute is SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE

4)Set booldata value as true for attribute id SAI_PORT_ATTR_AUTO_NEG_MODE

5)Set port attribute for the port method table with port_id , attribute as parameters

6)Verify return value is SAI_STATUS_SUCCESS

7)For attribute id SAI_PORT_ATTR_AUTO_NEG_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

8)Verify return value is SAI_STATUS_SUCCESS

9)Verify booldata value is true

10)Set booldata value as false for attribute id SAI_PORT_ATTR_AUTO_NEG_MODE

11)Set port attribute for the port method table with port_id , attribute as parameters

12)Verify return value is SAI_STATUS_SUCCESS

13)For attribute id SAI_PORT_ATTR_AUTO_NEG_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

14)Verify return value is SAI_STATUS_SUCCESS

15)Verify booldata value is false

Test in all valid ports

Test case number : 14

Title : Port Full duplex

Purpose : Validates if port full duplex can be enabled for all the valid ports

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

2)Verify return value is SAI_STATUS_SUCCESS

3)Verify s32 value of the attribute is SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE

4)Set booldata value as true for attribute id SAI_PORT_ATTR_FULL_DUPLEX_MODE

5)Set port attribute for the port method table with port_id , attribute as parameters

6)Verify return value is SAI_STATUS_SUCCESS

7)For attribute id SAI_PORT_ATTR_FULL_DUPLEX_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

8)Verify return value is SAI_STATUS_SUCCESS

9)Verify booldata value is true

10)Set booldata value as false for attribute id SAI_PORT_ATTR_FULL_DUPLEX_MODE

11)Set port attribute for the port method table with port_id , attribute as parameters

12)Verify return value is SAI_STATUS_SUCCESS

13)For attribute id SAI_PORT_ATTR_FULL_DUPLEX_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

14)Verify return value is SAI_STATUS_SUCCESS

15)Verify booldata value is false

Test in all valid ports

Test case number : 15

Title : Hardware Lane List

Purpose : Validates and prints the HW lane list for all the valid ports in the switch

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_HW_LANE_LIST get port attributes with port_id , attr_count=1 , attribute list as parameters

2)Verify return value is SAI_STATUS_SUCCESS

3)Initialize sai_attr_get.value.u32list.count = 1

3)Initialize variable lane = 0

4)Get the value of sai_attr_get.value.u32list.list[lane] and increment lane till the sai_attr_get.value.u32list.count value


Test all valid ports


Test case number : 16

Title : Port Supported Breakout Modes

Purpose : Validates and prints the supported breakout modes for all the valid ports in the switch

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE_TYPE get port attributes with port_id , attr_count=1 , attribute list as parameters

2)Verify return value is SAI_STATUS_SUCCESS

3)Initialize sai_attr_get.value.s32list.count = SAI_PORT_BREAKOUT_MODE_TYPE_MAX

3)Initialize variable count = 0

4)Get the value of sai_attr_get.value.u32list.list[count] and increment count till the sai_attr_get.value.u32list.count value


Test all valid ports

Test case number : 17

Title : Port Current Breakout Modes

Purpose : Validates and prints the current breakout modes for all the valid ports in the switch

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE_TYPE get port attributes with port_id , attr_count=1 , attribute list as parameters

2)Verify return value is SAI_STATUS_SUCCESS

3)Get s32 value of the attribute

Test all valid ports

Test case number : 18

Title : Port Update DSCP

Purpose : Validates if the port update DSCP can be enabled on the port

Topology:  Standalone unit

Procedure:
1)Set booldata value as true for attribute id SAI_PORT_ATTR_UPDATE_DSCP

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_UPDATE_DSCP get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true


Test case number : 19

Title : Flood Storm Control

Purpose : Validates if the flood storm control can be enabled on the port

Topology:  Standalone unit

Procedure:

1)Set booldata value as true for attribute id SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true


Test case number : 20

Title : Broadcast Storm Control

Purpose : Validates if the broadcast storm control can be enabled on the port

Topology:  Standalone unit

Procedure:

1)Set booldata value as true for attribute id SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify booldata value is true


Test case number : 21

Title : Multicast Storm Control

Purpose : Validates if the multicast storm control can be enabled on the port

Topology:  Standalone unit

Procedure:

sai_attr_set.id = SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID

sai_attr_set.value.booldata = true

sai_port_api_table->set_port_attribute(gport_id, &sai_attr_set)

sai_attr_get.id = SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID

sai_port_api_table->get_port_attribute(gport_id, 1, &sai_attr_get)

ASSERT_TRUE(sai_attr_get.value.booldata)


Test case number : 22

Title : Port MTU

Purpose : Validates if the port MTU can be set to 1000 for all valid ports in the switch

Topology:  Standalone unit

Procedure:

1)Set u32 value as 1000 for attribute id SAI_PORT_ATTR_MTU

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_MTU get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify u32 value is 1000

Test all valid ports


Test case number : 23

Title : Port Max MTU

Purpose : Validate max port MTU for all valid ports in the switch

Topology:  Standalone unit

Procedure:
1)For attribute id SAI_SWITCH_ATTR_PORT_MAX_MTU get switch attribute with parameters switch_id=0, attr_count=1, attribute list

2)Verify return value is SAI_STATUS_SUCCESS

3)Assign s32 value of attribute to variable max_mtu

4)Set u32 value as max_mtu for attribute id SAI_PORT_ATTR_MTU

5)Set port attribute for the port method table with port_id , attribute as parameters

6)Verify return value is SAI_STATUS_SUCCESS

7)For attribute id SAI_PORT_ATTR_TYPE get port attribute with parameters  port_id, attr_count=1, attribute list

8)Verify return value is SAI_STATUS_SUCCESS

9)Verify s32 value is max_mtu

Test all valid ports

Test case number : 24

Title : Supported Port media type

Purpose : Set all supported port media type one by one and verify configured value is set by reading back the media type

Topology:  Standalone unit

Procedure:

1)Set u32 value as SAI_PORT_MEDIA_TYPE_NOT_PRESENT for attribute id SAI_PORT_ATTR_MEDIA_TYPE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_MEDIA_TYPE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify u32 value is SAI_PORT_MEDIA_TYPE_NOT_PRESENT

Verify for all media type values

SAI_PORT_MEDIA_TYPE_NOT_PRESENT

SAI_PORT_MEDIA_TYPE_UNKNOWN

SAI_PORT_MEDIA_TYPE_FIBER

SAI_PORT_MEDIA_TYPE_COPPER

Test case number : 25

Title : Get Port Statistics

Purpose : Port All Statistics Get: Tests only the support of statistics counters;not the stats collection functionality

Topology:  Standalone unit

Procedure:

1)Set variable counter_ids[0] = SAI_PORT_STAT_IF_IN_OCTETS

2)For counter SAI_PORT_STAT_IF_IN_OCTETS call get_port_starts with port_id , array of counter ids=counter_ids , number of counters=1 as in parameters ,
array of resulting counter values as out parameter

3)Verify return value is SAI_STATUS_SUCCESS

Repeat the test case for all counters and all ports

Test case number : 26

Title : Clear Port Statistics

Purpose : Clear port's statistic counters one by one

Topology:  Standalone unit

Procedure:

 1)Set variable counter_ids[0] = SAI_PORT_STAT_IF_IN_OCTETS

2)For counter SAI_PORT_STAT_IF_IN_OCTETS call clear_port_stats with port_id , array of counter ids=counter_ids , number of counters=1 as parameters

3)Verify return value is SAI_STATUS_SUCCESS

Repeat the test case for all counters and all ports

Test case number : 27

Title : Clear Port All Statistics

Purpose : Clear port's all statistic counters at one shot

Topology:  Standalone unit

Procedure:

1)For port method table call clear_port_all_stats with port_id as parameter

2)Verify return value is SAI_STATUS_SUCCESS

Repeat the test case for all ports

Test case number : 28

Title : Get Port Egress Block count

Purpose : Validate Get Egress Block count Port list

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST set objlist value as variable egress_block_port_list

2)Get port attribute for the port method table calling get_port_attribute with port_id , attr_count=1 , attribute list as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)Get objlist.count value of the attribute


Test case number : 29

Title : Set Port Egress Block Port list

Purpose : Validate Set Egress Block Port list

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST set objlist value as variable egress_block_port_list

2)Get port attribute for the port method table calling get_port_attribute with port_id , attr_count=1 , attribute list as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)Get objlist.count value of the attribute and set it to variable egress_block_port_count

5)Set objlist value as egress_block_port_list for attribute id SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST with port_id , attribute list as parameters

6)Get port attribute for the port method table calling get_port_attribute with port_id , attr_count=1 , attribute list as parameters

7)Verify return value is SAI_STATUS_SUCCESS

8)set objlist.count value of the attribute and is equal to egress_block_port_count + 1

Test case number : 30

Title : Get Port Egress Block Port list for all ports

Purpose : Validate Get Port Egress Block Port list for all ports

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST set objlist value as variable egress_block_port_list

2)Get port attribute for the port method table calling get_port_attribute with port_id , attr_count=1 , attribute list as parameters

3)Verify return value is SAI_STATUS_SUCCESS

Test for all ports

Test case number : 31

Title : Clear Port Egress Block Port list

Purpose : Validate Port Egress Block Port list

Topology:  Standalone unit

Procedure:

1)Set value of object list variable egress_block_port_list.count = 0

2)For attribute id SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST set objlist value as egress_block_port_list

3)Set objlist value as egress_block_port_list for attribute id SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST with port_id , attribute list as parameters

4)Get port attribute for the port method table calling get_port_attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)et objlist.count value of the attribute and is equal to 0


Test case number : 32

Title : Port Flow Control

Purpose : Validates the port flow control can be enabled

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

2)Verify return value is SAI_STATUS_SUCCESS

3)Verify u32 value is SAI_PORT_FLOW_CONTROL_MODE_DISABLE

4)Set u32 value as SAI_PORT_FLOW_CONTROL_MODE_BOTH_ENABLE for attribute id SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE

5)Set port attribute for the port method table with port_id , attribute as parameters

6)Verify return value is SAI_STATUS_SUCCESS

7)For attribute id SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

8)Verify return value is SAI_STATUS_SUCCESS

9)Verify u32 value is SAI_PORT_FLOW_CONTROL_MODE_BOTH_ENABLE


Test case number : 33

Title : PFC bitmap test

Purpose : Validate PFC bitmap

Topology:  Standalone unit

Procedure:

1)For attribute id SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL get port attribute with port_id , attr_count=1 , attribute list as parameters

2)Verify return value is SAI_STATUS_SUCCESS

3)Verify u8 value is 0

4)Set u8 value as 0x4f for attribute id SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL

5)Set port attribute for the port method table with port_id , attribute as parameters

6)Verify return value is SAI_STATUS_SUCCESS

7)For attribute id SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL get port attribute with port_id , attr_count=1 , attribute list as parameters

8)Verify return value is SAI_STATUS_SUCCESS

9)Verify u8 value is 0x4f


Test case number : 34

Title : Disable port

Purpose : Verify disable port

Topology:  Standalone unit

Procedure:

1)Set booldata value as false for attribute id SAI_PORT_ATTR_ADMIN_STATE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

Test for all ports

Test case number : 35

Title : 4x10 Breakout

Purpose : Validate 4x10 port breakout

Topology:  Standalone unit

Procedure:

1)Set value of u32list.list as hardware lane list value for attribute id SAI_PORT_ATTR_HW_LANE_LIST

2)Set u32 value as SAI_PORT_SPEED_TEN_GIG for attribute id SAI_PORT_ATTR_SPEED

3)Create port and initialize port configs

4)Verify return value is SAI_STATUS_SUCCESS

Test for all valid ports

Test case number : 36

Title : 1x40 Breakin

Purpose : Validate 1x40 port breakin

Topology:  Standalone unit

Procedure:

1)Set value of u32list.list as hardware lane list value for attribute id SAI_PORT_ATTR_HW_LANE_LIST

2)Set u32 value as SAI_PORT_SPEED_FORTY_GIG for attribute id SAI_PORT_ATTR_SPEED

3)Create port and initialize port configs

4)Verify return value is SAI_STATUS_SUCCESS

Test for all valid ports

Test case number : 37

Title : FEC CL91 on 100G

Purpose : Validate set and get of FEC CL91 on 100G

Topology:  Standalone unit

Procedure:

1)Set s32 value as SAI_PORT_FEC_MODE_RS for attribute id SAI_PORT_ATTR_FEC_MODE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_FEC_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is SAI_PORT_FEC_MODE_RS

Test for all 100G ports

Test case number : 38

Title : FEC CL74 on 25G

Purpose : Validate set and get of FEC CL74 on 25G

Topology:  Standalone unit

Procedure:

1)Set s32 value as SAI_PORT_FEC_MODE_FC for attribute id SAI_PORT_ATTR_FEC_MODE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_FEC_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is SAI_PORT_FEC_MODE_FC

Test for all 25G ports

Test case number : 39

Title : FEC CL74 on 50G

Purpose : Validate set and get of FEC CL74 on 50G

Topology:  Standalone unit

Procedure:

1)Set s32 value as SAI_PORT_FEC_MODE_FC for attribute id SAI_PORT_ATTR_FEC_MODE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_FEC_MODE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is SAI_PORT_FEC_MODE_FC

Test for all 50G ports

Test case number : 40

Title : OUI on 25G

Purpose : Validate set and get of OUI on 25G

Topology:  Standalone unit

Procedure:

1)Set s32 value as SAI_PORT_OUI_CODE_1 for attribute id SAI_PORT_ATTR_ADVERTISED_OUI_CODE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_ADVERTISED_OUI_CODE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is SAI_PORT_OUI_CODE_1

Test for all 25G ports

Test case number : 41

Title : OUI on 50G

Purpose : Validate set and get of OUI on 50G

Topology:  Standalone unit

Procedure:

1)Set s32 value as SAI_PORT_OUI_CODE_2 for attribute id SAI_PORT_ATTR_ADVERTISED_OUI_CODE

2)Set port attribute for the port method table with port_id , attribute as parameters

3)Verify return value is SAI_STATUS_SUCCESS

4)For attribute id SAI_PORT_ATTR_ADVERTISED_OUI_CODE get port attribute with port_id , attr_count=1 , attribute list as parameters

5)Verify return value is SAI_STATUS_SUCCESS

6)Verify s32 value is SAI_PORT_OUI_CODE_2

Test for all 50G ports