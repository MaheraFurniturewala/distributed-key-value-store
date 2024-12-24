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

if __name__ == "__main__":
    app.run(debug=True, port=4000)
