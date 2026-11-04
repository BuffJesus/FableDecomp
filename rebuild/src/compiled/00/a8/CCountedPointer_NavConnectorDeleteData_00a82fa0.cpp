// CCountedPointer<CNavConnectorNode>::DeleteData (retail 0x00A82FA0, Ego_r 0x00BC4940): the shared block's delete
// function for the nav graph's connectors - destroy the connector (unlinking it from both leaves) and free it.
#include "fable_counted_pointer.h"
#include "fable_nav_connector_node.h"

template void CCountedPointer<CNavConnectorNode>::DeleteData(void*);
