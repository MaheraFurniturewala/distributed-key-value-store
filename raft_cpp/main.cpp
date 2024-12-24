#include "RaftNode.h"

int main() {
    const int totalNodes = 3;

    // Simulate 3 nodes in the cluster
    RaftNode node1(1, totalNodes);
    RaftNode node2(2, totalNodes);
    RaftNode node3(3, totalNodes);

    // Start an election from Node 1
    node1.startElection();

    return 0;
}
