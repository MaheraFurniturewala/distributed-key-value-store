#include "RaftNode.h"

int main() {
    const int totalNodes = 3;

    // Create a node
    RaftNode node1(1, totalNodes);

    // Simulate an election
    node1.startElection();

    // Save state
    node1.saveState();

    // Restart the node
    RaftNode restartedNode(1, totalNodes);

    return 0;
}
