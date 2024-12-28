#include "RaftNode.h"

int main() {
    const int totalNodes = 3;

    RaftNode node1(1, totalNodes);
    RaftNode node2(2, totalNodes);
    RaftNode node3(3, totalNodes);

    node1.becomeLeader();
    node1.sendHeartbeats();
    node1.appendEntry("SET key1 value1");

    return 0;
}
