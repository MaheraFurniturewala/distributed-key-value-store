class KeyValueStore:
    def __init__(self):
        """
        Initialize the key-value store.
        This creates an empty dictionary to store the data.
        """
        self.store = {}

    def set(self, key, value):
        """
        Add or update a key-value pair in the store.
        :param key: The key (must be unique).
        :param value: The value to store.
        """
        self.store[key] = value
        print(f"SET: Key '{key}' set to '{value}'.")

    def get(self, key):
        """
        Retrieve the value for a given key.
        :param key: The key to look up.
        :return: The value if the key exists, or an error message.
        """
        if key in self.store:
            return self.store[key]
        else:
            return f"ERROR: Key '{key}' not found."

    def delete(self, key):
        """
        Remove a key-value pair from the store.
        :param key: The key to delete.
        """
        if key in self.store:
            del self.store[key]
            print(f"DELETE: Key '{key}' has been removed.")
        else:
            print(f"ERROR: Key '{key}' not found.")

# Command-Line Interface for the Key-Value Store
def main():
    kv_store = KeyValueStore()
    print("Simple Key-Value Store. Type 'help' for commands.")

    while True:
        command = input("> ").strip().lower()
        if command == "exit":
            print("Exiting Key-Value Store. Goodbye!")
            break
        elif command.startswith("set"):
            _, key, value = command.split(maxsplit=2)
            kv_store.set(key, value)
        elif command.startswith("get"):
            _, key = command.split(maxsplit=1)
            print(kv_store.get(key))
        elif command.startswith("delete"):
            _, key = command.split(maxsplit=1)
            kv_store.delete(key)
        elif command == "help":
            print("Commands:")
            print("  set <key> <value> - Add or update a key-value pair.")
            print("  get <key> - Retrieve the value for a key.")
            print("  delete <key> - Remove a key-value pair.")
            print("  exit - Quit the program.")
        else:
            print("ERROR: Unknown command. Type 'help' for a list of commands.")

if __name__ == "__main__":
    main()
