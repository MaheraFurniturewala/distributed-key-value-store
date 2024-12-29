#ifndef RAFTNODE_H
#define RAFTNODE_H

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
using namespace std;

// State Definitions
enum State {
    LEADER,
    FOLLOWER,
    CANDIDATE
};

// RequestVote Structure
struct RequestVote {
    int term;         // Candidate's current term
    int candidateID;  // Candidate's node ID
    int lastLogIndex; // Index of candidate's last log entry
    int lastLogTerm;  // Term of candidate's last log entry
};

class RaftNode {
private:
    int nodeID;              // Unique ID for the node
    State currentState;      // Current state (LEADER, FOLLOWER, CANDIDATE)
    int currentTerm;         // Current term in RAFT
    int votedFor;            // ID of the node this node voted for in the current term
    vector<string> logEntries; // Log of commands replicated on this node
    int totalNodes;          // Total number of nodes in the cluster

    string stateFile;        // File for persistent state

    mutex mtx;               // Mutex to protect shared state
    condition_variable cv;   // Condition variable for state changes

public:
    // Constructor
    RaftNode(int id, int totalNodes);

    // Election Functions
    void startElection();
    void sendRequestVote();
    bool handleRequestVote(const RequestVote& request, int voterID);

    // State Transition Functions
    void becomeLeader();
    void becomeFollower();

    // Utility
    void logState();

    void sendHeartbeats();
    void appendEntry(const string& command); // Append a log entry and replicate it to followers
    bool handleAppendEntries(int term, const string& command); // Handle incoming AppendEntries

    // Persistent state helper functions
    void saveState();        // Save persistent state to disk
    void loadState();        // Load persistent state from disk
};

#endif
