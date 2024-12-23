from flask import Flask, request, jsonify

class KeyValueStore:
    def __init__(self):
        self.store = {}

    def set(self, key, value):
        self.store[key] = value
        return f"Key '{key}' set to '{value}'."

    def get(self, key):
        if key in self.store:
            return self.store[key]
        else:
            return f"ERROR: Key '{key}' not found."

    def delete(self, key):
        if key in self.store:
            del self.store[key]
            return f"Key '{key}' has been removed."
        else:
            return f"ERROR: Key '{key}' not found."


# Flask application
app = Flask(__name__)
kv_store = KeyValueStore()


@app.route('/set', methods=['POST'])
def set_key():
    data = request.json
    key = data.get('key')
    value = data.get('value')
    if not key or not value:
        return jsonify({"error": "Key and value are required!"}), 400
    result = kv_store.set(key, value)
    return jsonify({"message": result})


@app.route('/get/<key>', methods=['GET'])
def get_key(key):
    result = kv_store.get(key)
    return jsonify({"result": result})


@app.route('/delete/<key>', methods=['DELETE'])
def delete_key(key):
    result = kv_store.delete(key)
    return jsonify({"message": result})


if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print("Usage: python kv_store.py <port>")
        sys.exit(1)
    port = int(sys.argv[1])
    app.run(debug=True, port=port)

