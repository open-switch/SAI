LIBRARIES=sai-stubs
SAI_LIB=-lsai-0.9.6
CXXFLAGS+=-std=c++11

sai-stubs_SRCS+=sai.c
sai-stubs_LDFLAGS=-zdefs


UNIT_TEST += sai_switch_unit_test
sai_switch_unit_test_SRCS= unit_test/sai_infra_unit_test.cpp unit_test/sai_switch_unit_test.cpp unit_test/port/sai_port_breakout_test_utils.cpp
sai_switch_unit_test_LDFLAGS= ${SAI_LIB}
sai_switch_unit_test_CPPFLAGS=-Iunit_test/port

UNIT_TEST += sai_stp_unit_test
sai_stp_unit_test_SRCS= unit_test/switching/sai_stp_unit_test.cpp
sai_stp_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_port_unit_test
sai_port_unit_test_SRCS= unit_test/port/sai_port_unit_test.cpp unit_test/port/sai_port_breakout_test_utils.cpp
sai_port_unit_test_SRCS+= unit_test/bridge/sai_bridge_unit_test_utils.cpp
sai_port_unit_test_LDFLAGS= ${SAI_LIB}
sai_port_unit_test_CPPFLAGS= -Iunit_test/switching -Iunit_test/bridge

UNIT_TEST += sai_fdb_unit_test
sai_fdb_unit_test_SRCS+= unit_test/switching/sai_fdb_unit_test.cpp
sai_fdb_unit_test_SRCS+= unit_test/bridge/sai_bridge_unit_test_utils.cpp
sai_fdb_unit_test_CPPFLAGS= -Iunit_test/switching -Iunit_test/bridge
sai_fdb_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_vlan_unit_test
sai_vlan_unit_test_SRCS+= unit_test/switching/sai_vlan_unit_test.cpp
sai_vlan_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_lag_unit_test
sai_lag_unit_test_SRCS+= unit_test/switching/sai_lag_unit_test.cpp
sai_lag_unit_test_SRCS+= unit_test/bridge/sai_bridge_unit_test_utils.cpp
sai_lag_unit_test_CPPFLAGS= -Iunit_test/switching -Iunit_test/bridge
sai_lag_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_l3_vrf_unit_test
sai_l3_vrf_unit_test_SRCS= unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp unit_test/routing/sai_l3_vrf_unit_test.cpp
sai_l3_vrf_unit_test_LDFLAGS= ${SAI_LIB}
sai_l3_vrf_unit_test_CPPFLAGS=-Iunit_test/bridge

UNIT_TEST += sai_l3_rif_unit_test
sai_l3_rif_unit_test_SRCS= unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp unit_test/routing/sai_l3_rif_unit_test.cpp
sai_l3_rif_unit_test_LDFLAGS= ${SAI_LIB}
sai_l3_rif_unit_test_CPPFLAGS=-Iunit_test/bridge

UNIT_TEST += sai_l3_route_unit_test
sai_l3_route_unit_test_SRCS= unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp unit_test/routing/sai_l3_route_unit_test.cpp
sai_l3_route_unit_test_LDFLAGS= ${SAI_LIB}
sai_l3_route_unit_test_CPPFLAGS=-Iunit_test/bridge

UNIT_TEST += sai_l3_nexthop_unit_test
sai_l3_nexthop_unit_test_SRCS= unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp unit_test/routing/sai_l3_nexthop_unit_test.cpp
sai_l3_nexthop_unit_test_LDFLAGS= ${SAI_LIB}
sai_l3_nexthop_unit_test_CPPFLAGS=-Iunit_test/bridge

UNIT_TEST += sai_l3_neighbor_unit_test
sai_l3_neighbor_unit_test_SRCS= unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp unit_test/routing/sai_l3_neighbor_unit_test.cpp
sai_l3_neighbor_unit_test_LDFLAGS= ${SAI_LIB}
sai_l3_neighbor_unit_test_CPPFLAGS=-Iunit_test/bridge

UNIT_TEST += sai_l3_nexthopgroup_unit_test
sai_l3_nexthopgroup_unit_test_SRCS= unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp unit_test/routing/sai_l3_nexthopgroup_unit_test.cpp
sai_l3_nexthopgroup_unit_test_LDFLAGS= ${SAI_LIB}
sai_l3_nexthopgroup_unit_test_CPPFLAGS=-Iunit_test/bridge

UNIT_TEST += sai_acl_table_unit_test
sai_acl_table_unit_test_SRCS= unit_test/acl/sai_acl_unit_test_utils.cpp unit_test/acl/sai_acl_table_unit_test.cpp unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp
sai_acl_table_unit_test_LDFLAGS= ${SAI_LIB}
sai_acl_table_unit_test_CPPFLAGS=-Iunit_test/routing
sai_acl_table_unit_test_CPPFLAGS+=-Iunit_test/bridge

UNIT_TEST += sai_acl_rule_unit_test
sai_acl_rule_unit_test_SRCS= unit_test/acl/sai_acl_unit_test_utils.cpp unit_test/acl/sai_acl_rule_unit_test.cpp unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp
sai_acl_rule_unit_test_SRCS+= unit_test/udf/sai_udf_unit_test_utils.cpp
sai_acl_rule_unit_test_LDFLAGS= ${SAI_LIB}
sai_acl_rule_unit_test_CPPFLAGS=-Iunit_test/routing
sai_acl_rule_unit_test_CPPFLAGS+=-Iunit_test/udf
sai_acl_rule_unit_test_CPPFLAGS+=-Iunit_test/bridge

UNIT_TEST += sai_acl_counter_unit_test
sai_acl_counter_unit_test_SRCS= unit_test/acl/sai_acl_unit_test_utils.cpp unit_test/acl/sai_acl_counter_unit_test.cpp unit_test/bridge/sai_bridge_unit_test_utils.cpp
sai_acl_counter_unit_test_LDFLAGS= ${SAI_LIB}
sai_acl_counter_unit_test_CPPFLAGS= -Iunit_test/bridge

UNIT_TEST += sai_mirror_unit_test
sai_mirror_unit_test_SRCS = unit_test/mirroring/sai_mirror_unit_test.cpp unit_test/mirroring/sai_mirror_unit_test_utils.cpp unit_test/port/sai_port_breakout_test_utils.cpp
sai_mirror_unit_test_SRCS+= unit_test/bridge/sai_bridge_unit_test_utils.cpp
sai_mirror_unit_test_LDFLAGS= ${SAI_LIB}
sai_mirror_unit_test_CPPFLAGS=-Iunit_test/port -Iunit_test/bridge

UNIT_TEST += sai_hostif_unit_test
sai_hostif_unit_test_SRCS= unit_test/hostintf/sai_hostif_unit_test.cpp
sai_hostif_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_samplepacket_unit_test
sai_samplepacket_unit_test_SRCS= unit_test/samplepacket/sai_samplepacket_unit_test.cpp unit_test/samplepacket/sai_samplepacket_unit_test_utils.cpp
sai_samplepacket_unit_test_SRCS+=unit_test/port/sai_port_breakout_test_utils.cpp
sai_samplepacket_unit_test_LDFLAGS= ${SAI_LIB}
sai_samplepacket_unit_test_CPPFLAGS=-Iunit_test/port

UNIT_TEST += sai_qos_scheduler_group_unit_test
sai_qos_scheduler_group_unit_test_SRCS = unit_test/qos/sai_qos_unit_test_utils.cpp unit_test/qos/sai_qos_scheduler_group_unit_test.cpp
sai_qos_scheduler_group_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_qos_queue_unit_test
sai_qos_queue_unit_test_SRCS = unit_test/qos/sai_qos_unit_test_utils.cpp unit_test/qos/sai_qos_queue_unit_test.cpp
sai_qos_queue_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_qos_scheduler_unit_test
sai_qos_scheduler_unit_test_SRCS = unit_test/qos/sai_qos_unit_test_utils.cpp unit_test/qos/sai_qos_scheduler_unit_test.cpp
sai_qos_scheduler_unit_test_LDFLAGS= ${SAI_LIB}


UNIT_TEST += sai_qos_maps_unit_test
sai_qos_maps_unit_test_SRCS = unit_test/qos/sai_qos_unit_test_utils.cpp unit_test/qos/sai_qos_maps_unit_test.cpp
sai_qos_maps_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_qos_policer_unit_test
sai_qos_policer_unit_test_SRCS = unit_test/qos/sai_qos_unit_test_utils.cpp unit_test/qos/sai_policer_unit_test.cpp
sai_qos_policer_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_qos_wred_unit_test
sai_qos_wred_unit_test_SRCS = unit_test/qos/sai_qos_unit_test_utils.cpp unit_test/qos/sai_wred_unit_test.cpp
sai_qos_wred_unit_test_SRCS+=unit_test/acl/sai_acl_unit_test_utils.cpp unit_test/bridge/sai_bridge_unit_test_utils.cpp
sai_qos_wred_unit_test_LDFLAGS= ${SAI_LIB}
sai_qos_wred_unit_test_CPPFLAGS = -Iunit_test/acl -Iunit_test/bridge

UNIT_TEST += sai_qos_buffer_unit_test
sai_qos_buffer_unit_test_SRCS = unit_test/qos/sai_qos_unit_test_utils.cpp unit_test/qos/sai_qos_buffer_unit_test.cpp
sai_qos_buffer_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_qos_buffer_tile_unit_test
sai_qos_buffer_tile_unit_test_SRCS+= unit_test/qos/sai_qos_unit_test_utils.cpp
sai_qos_buffer_tile_unit_test_CPPFLAGS= -Iunit_test/qos
sai_qos_buffer_tile_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_udf_unit_test
sai_udf_unit_test_SRCS = unit_test/udf/sai_udf_unit_test.cpp unit_test/udf/sai_udf_unit_test_utils.cpp
sai_udf_unit_test_LDFLAGS= ${SAI_LIB}

UNIT_TEST += sai_hash_unit_test
sai_hash_unit_test_SRCS = unit_test/hash/sai_hash_unit_test.cpp unit_test/hash/sai_hash_unit_test_utils.cpp unit_test/udf/sai_udf_unit_test_utils.cpp
sai_hash_unit_test_LDFLAGS= ${SAI_LIB}
sai_hash_unit_test_CPPFLAGS=-Iunit_test/udf

UNIT_TEST += sai_ip_tunnel_unit_test
sai_ip_tunnel_unit_test_SRCS = unit_test/tunnel/sai_ip_tunnel_unit_test.cpp
sai_ip_tunnel_unit_test_SRCS += unit_test/tunnel/sai_tunnel_unit_test_utils.cpp unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp
sai_ip_tunnel_unit_test_LDFLAGS= ${SAI_LIB}
sai_ip_tunnel_unit_test_CPPFLAGS=-Iunit_test/routing
sai_ip_tunnel_unit_test_CPPFLAGS+=-Iunit_test/bridge

UNIT_TEST += sai_bridge_unit_test
sai_bridge_unit_test_SRCS+= unit_test/bridge/sai_bridge_unit_test_utils.cpp unit_test/bridge/sai_bridge_unit_test.cpp
sai_bridge_unit_test_LDFLAGS= ${SAI_LIB}
sai_bridge_unit_test_CPPFLAGS= -Iunit_test/bridge

UNIT_TEST += sai_vxlan_tunnel_unit_test
sai_vxlan_tunnel_unit_test_SRCS = unit_test/tunnel/sai_vxlan_tunnel_unit_test.cpp
sai_vxlan_tunnel_unit_test_SRCS += unit_test/tunnel/sai_tunnel_unit_test_utils.cpp unit_test/routing/sai_l3_unit_test_utils.cpp unit_test/bridge/sai_bridge_unit_test_utils.cpp
sai_vxlan_tunnel_unit_test_SRCS += unit_test/tunnel/sai_vxlan_tunnel_unit_test_utils.cpp
sai_vxlan_tunnel_unit_test_LDFLAGS = ${SAI_LIB}
sai_vxlan_tunnel_unit_test_CPPFLAGS = -Iunit_test/routing
sai_vxlan_tunnel_unit_test_CPPFLAGS+=-Iunit_test/bridge

UNIT_TEST += sai_l2mc_unit_test
sai_l2mc_unit_test_SRCS+= unit_test/switching/sai_l2mc_unit_test.cpp
sai_l2mc_unit_test_LDFLAGS= ${SAI_LIB}

include ${MAKE_INC}/workspace.mak

install-local:${BUILD_CURDIR_libs}/libsai-stubs.so
	ln -s ${BUILD_CURDIR_libs}/libsai-stubs.so ${BUILD_CURDIR_libs}/libsai-0.9.6.so
