import hashlib
from bisect import bisect_right

class ConsistentHashing:
    def __init__(self, nodes=None, replicas=3):
        """
        Initialize the consistent hashing ring.
        :param nodes: List of initial nodes (e.g., ["Node1", "Node2"]).
        :param replicas: Number of virtual replicas for each node.
        """
        self.replicas = replicas
        self.ring = {}
        self.sorted_keys = []

        if nodes:
            for node in nodes:
                self.add_node(node)

    def _hash(self, key):
        """
        Generate a hash for a given key.
        :param key: The key to hash.
        :return: An integer hash value.
        """
        return int(hashlib.md5(key.encode('utf-8')).hexdigest(), 16)

    def add_node(self, node):
        """
        Add a node to the ring with replicas.
        :param node: The node to add.
        """
        for i in range(self.replicas):
            node_replica = f"{node}:{i}"
            hashed_key = self._hash(node_replica)
            self.ring[hashed_key] = node
            self.sorted_keys.append(hashed_key)

        self.sorted_keys.sort()

    def remove_node(self, node):
        """
        Remove a node and its replicas from the ring.
        :param node: The node to remove.
        """
        for i in range(self.replicas):
            node_replica = f"{node}:{i}"
            hashed_key = self._hash(node_replica)
            del self.ring[hashed_key]
            self.sorted_keys.remove(hashed_key)

    def get_node(self, key):
        """
        Get the node responsible for the given key.
        :param key: The key to look up.
        :return: The node responsible for the key.
        """
        if not self.ring:
            return None

        hashed_key = self._hash(key)
        index = bisect_right(self.sorted_keys, hashed_key) % len(self.sorted_keys)
        node_key = self.sorted_keys[index]
        return self.ring[node_key]
