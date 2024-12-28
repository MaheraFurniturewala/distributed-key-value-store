#include "RaftNode.h"

RaftNode::RaftNode(int id, int totalNodes) 
    : nodeID(id), currentState(FOLLOWER), currentTerm(0), votedFor(-1), totalNodes(totalNodes) {}

void RaftNode::startElection() {
    unique_lock<mutex> lock(mtx);
    currentState = CANDIDATE;
    currentTerm += 1; // Increment the term
    votedFor = nodeID; // Vote for self

    cout << "Node " << nodeID << " started an election for term " << currentTerm << endl;

    sendRequestVote();
}

void RaftNode::sendRequestVote() {
    cout << "Node " << nodeID << " sending RequestVote for term " << currentTerm << endl;

    RequestVote request = {currentTerm, nodeID, (int)logEntries.size(), currentTerm};

    for (int otherNodeID = 1; otherNodeID <= totalNodes; ++otherNodeID) {
        if (otherNodeID != nodeID) {
            cout << "Node " << nodeID << " requesting vote from Node " << otherNodeID << endl;

            bool voteGranted = handleRequestVote(request, otherNodeID);
            cout << "Node " << otherNodeID << " vote response: " << (voteGranted ? "Granted" : "Rejected") << endl;
        }
    }
}

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

void RaftNode::becomeLeader() {
    unique_lock<mutex> lock(mtx);
    currentState = LEADER;
    cout << "Node " << nodeID << " became the leader for term " << currentTerm << endl;
}

void RaftNode::becomeFollower() {
    unique_lock<mutex> lock(mtx);
    currentState = FOLLOWER;
    cout << "Node " << nodeID << " became a follower" << endl;
}

void RaftNode::sendHeartbeats() {
    unique_lock<mutex> lock(mtx);

    if (currentState != LEADER) {
        cout << "Node " << nodeID << " is not a leader, cannot send heartbeats." << endl;
        return;
    }

    cout << "Node " << nodeID << " sending heartbeats for term " << currentTerm << endl;

    for (int otherNodeID = 1; otherNodeID <= totalNodes; ++otherNodeID) {
        if (otherNodeID != nodeID) {
            cout << "Node " << nodeID << " sent heartbeat to Node " << otherNodeID << endl;

            bool success = handleAppendEntries(currentTerm, "");
            cout << "Node " << otherNodeID << " handled heartbeat: " << (success ? "Success" : "Failure") << endl;
        }
    }
}

void RaftNode::appendEntry(const string& command) {
    unique_lock<mutex> lock(mtx);

    if (currentState != LEADER) {
        cout << "Node " << nodeID << " is not a leader, cannot append entry." << endl;
        return;
    }

    logEntries.push_back(command);
    cout << "Node " << nodeID << " appended command '" << command << "' to its log." << endl;

    for (int otherNodeID = 1; otherNodeID <= totalNodes; ++otherNodeID) {
        if (otherNodeID != nodeID) {
            cout << "Node " << nodeID << " replicating command '" << command << "' to Node " << otherNodeID << endl;

            handleAppendEntries(currentTerm, command);
        }
    }
}

bool RaftNode::handleAppendEntries(int term, const string& command) {
    cout << "Node " << nodeID << " entered handleAppendEntries." << endl;

    // Mutex removed since the caller (sendHeartbeats or appendEntry) already holds the lock

    if (term > currentTerm) {
        currentTerm = term;
        currentState = FOLLOWER;
        votedFor = -1;
        cout << "Node " << nodeID << " stepped down to FOLLOWER for term " << currentTerm << endl;
    }

    cout << "Node " << nodeID << " received command: " << (command.empty() ? "Heartbeat" : command) << endl;

    if (!command.empty()) {
        logEntries.push_back(command);
        cout << "Node " << nodeID << " appended command '" << command << "' to its log." << endl;
    }

    cout << "Node " << nodeID << " leaving handleAppendEntries." << endl;
    return true;
}
