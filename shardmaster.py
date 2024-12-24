from flask import Flask, request, jsonify
from consistent_hashing import ConsistentHashing
import requests

app = Flask(__name__)

# Initialize consistent hashing with initial nodes
initial_nodes = ["http://127.0.0.1:5000", "http://127.0.0.1:5001", "http://127.0.0.1:5002"]
ch = ConsistentHashing(initial_nodes)

@app.route('/set', methods=['POST'])
def set_key():
    data = request.json
    key = data.get('key')
    value = data.get('value')

    if not key or not value:
        return jsonify({"error": "Key and value are required!"}), 400

    node = ch.get_node(key)
    response = requests.post(f"{node}/set", json=data)
    return jsonify(response.json())

@app.route('/get/<key>', methods=['GET'])
def get_key(key):
    node = ch.get_node(key)
    response = requests.get(f"{node}/get/{key}")
    return jsonify(response.json())

@app.route('/delete/<key>', methods=['DELETE'])
def delete_key(key):
    node = ch.get_node(key)
    response = requests.delete(f"{node}/delete/{key}")
    return jsonify(response.json())

@app.route('/add_node', methods=['POST'])
def add_node():
    """
    Add a new node to the consistent hashing ring.
    Example payload: { "node": "http://127.0.0.1:5003" }
    """
    data = request.json
    node = data.get('node')

    if not node:
        return jsonify({"error": "Node is required!"}), 400

    ch.add_node(node)
    return jsonify({"message": f"Node {node} added successfully."})


@app.route('/remove_node', methods=['POST'])
def remove_node():
    """
    Remove a node from the consistent hashing ring.
    Example payload: { "node": "http://127.0.0.1:5003" }
    """
    data = request.json
    node = data.get('node')

    if not node:
        return jsonify({"error": "Node is required!"}), 400

    ch.remove_node(node)
    return jsonify({"message": f"Node {node} removed successfully."})


if __name__ == "__main__":
    app.run(debug=True, port=4000)
