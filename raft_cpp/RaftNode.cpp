#include "RaftNode.h"

// Constructor
RaftNode::RaftNode(int id, int totalNodes) 
    : nodeID(id), currentState(FOLLOWER), currentTerm(0), votedFor(-1), totalNodes(totalNodes) {}

// Start an Election
void RaftNode::startElection() {
    unique_lock<mutex> lock(mtx);
    currentState = CANDIDATE;
    currentTerm += 1; // Increment the term
    votedFor = nodeID; // Vote for self

    cout << "Node " << nodeID << " started an election for term " << currentTerm << endl;

    // Send RequestVote messages
    sendRequestVote();
}

// Send RequestVote Messages to Other Nodes
void RaftNode::sendRequestVote() {
    cout << "Node " << nodeID << " sending RequestVote for term " << currentTerm << endl;

    // Create RequestVote message
    RequestVote request = {currentTerm, nodeID, (int)logEntries.size(), currentTerm};

    // Simulate sending RequestVote to other nodes
    for (int otherNodeID = 1; otherNodeID <= totalNodes; ++otherNodeID) {
        if (otherNodeID != nodeID) {
            cout << "Node " << nodeID << " requesting vote from Node " << otherNodeID << endl;

            // Simulate handling the vote request locally
            bool voteGranted = handleRequestVote(request, otherNodeID);
            cout << "Node " << otherNodeID << " vote response: " << (voteGranted ? "Granted" : "Rejected") << endl;
        }
    }
}

// Handle Incoming RequestVote Messages
bool RaftNode::handleRequestVote(const RequestVote& request, int voterID) {
    cout << "Node " << voterID << " handling RequestVote for Node " << request.candidateID << " in term " << request.term << endl;

    if (request.term < currentTerm) {
        cout << "Node " << voterID << " rejects vote for Node " << request.candidateID << " (term too low)" << endl;
        return false;
    }

    if (request.term > currentTerm) {
        currentTerm = request.term;
        currentState = FOLLOWER; // Step down to follower
        votedFor = -1;
        cout << "Node " << voterID << " updated term to " << currentTerm << " and became FOLLOWER" << endl;
    }

    if (votedFor == -1 || votedFor == request.candidateID) {
        votedFor = request.candidateID;
        cout << "Node " << voterID << " votes for Node " << request.candidateID << endl;
        return true;
    }

    cout << "Node " << voterID << " rejects vote for Node " << request.candidateID << " (already voted)" << endl;
    return false;
}


// Transition to Leader State
void RaftNode::becomeLeader() {
    unique_lock<mutex> lock(mtx);
    currentState = LEADER;
    cout << "Node " << nodeID << " became the leader for term " << currentTerm << endl;
}

// Transition to Follower State
void RaftNode::becomeFollower() {
    unique_lock<mutex> lock(mtx);
    currentState = FOLLOWER;
    cout << "Node " << nodeID << " became a follower" << endl;
}
