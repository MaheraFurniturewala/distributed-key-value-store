import requests
from flask import Flask, request, jsonify

app = Flask(__name__)

# Node mapping: Assign keys to nodes based on their first letter
SHARD_MAP = {
    "A-F": "http://127.0.0.1:5000",
    "G-L": "http://127.0.0.1:5001",
    "M-Z": "http://127.0.0.1:5002"
}

def get_node_for_key(key):
    """
    Determine which node should handle the given key.
    :param key: The key to look up.
    :return: The URL of the node responsible for the key.
    """
    first_letter = key[0].upper()
    if "A" <= first_letter <= "F":
        return SHARD_MAP["A-F"]
    elif "G" <= first_letter <= "L":
        return SHARD_MAP["G-L"]
    elif "M" <= first_letter <= "Z":
        return SHARD_MAP["M-Z"]
    else:
        return None

@app.route('/set', methods=['POST'])
def set_key():
    data = request.json
    key = data.get('key')
    value = data.get('value')

    if not key or not value:
        return jsonify({"error": "Key and value are required!"}), 400

    node = get_node_for_key(key)
    if not node:
        return jsonify({"error": "Invalid key format!"}), 400

    # Forward the request to the appropriate node
    response = requests.post(f"{node}/set", json=data)
    return jsonify(response.json())

@app.route('/get/<key>', methods=['GET'])
def get_key(key):
    node = get_node_for_key(key)
    if not node:
        return jsonify({"error": "Invalid key format!"}), 400

    # Forward the request to the appropriate node
    response = requests.get(f"{node}/get/{key}")
    return jsonify(response.json())

@app.route('/delete/<key>', methods=['DELETE'])
def delete_key(key):
    node = get_node_for_key(key)
    if not node:
        return jsonify({"error": "Invalid key format!"}), 400

    # Forward the request to the appropriate node
    response = requests.delete(f"{node}/delete/{key}")
    return jsonify(response.json())

if __name__ == "__main__":
    app.run(debug=True, port=4000)
