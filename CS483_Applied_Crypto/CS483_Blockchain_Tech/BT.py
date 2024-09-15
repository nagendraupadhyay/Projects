# Nagendra Upadhyay. CS483/583 Blockchain Technology.
import hashlib
import random

# Check if the first 24 bits of the hash are all zeros
def is_proof_valid(byte_array):
    firstBits = (byte_array[0] << 16) | (byte_array[1] << 8) | byte_array[2]
    return firstBits == 0

# Reverse the byte order of the given byte array
def reverse_byte_order(byte_array):
    return byte_array[::-1]

# Print the byte array in hexadecimal format
def print_byte_array(byte_array):
    for i in byte_array:
        print(format(i, '02X'), end='')
    print()

# Convert a hexadecimal string to a byte array
def hex_string_to_byte_array(hex_string):
    length = len(hex_string)
    byte_array = bytearray(length // 2)
    for i in range(0, length, 2):
        byte_array[i // 2] = int(hex_string[i:i+2], 16)

    return byte_array

# Get user input for quote, previous hash, and difficulty
def get_block_data():
    quote = input("Enter Quote: ")
    prev_hash = input("Enter Previous Hash: ")
    difficulty = 24
    return quote, prev_hash, difficulty

# Generate a random nonce
def generate_nonce():
    firstThirtyBits = random.randint(0, (1 << 30) - 1)
    twoBits = random.randint(0, (1 << 2) - 1)
    bitRange = (firstThirtyBits << 2) | twoBits
    return bitRange

# Calculate the block hash and find a valid nonce
def calculate_block(quote, prev_hash, difficulty):
    proof_found = False
    quote_bytes = quote.encode('ascii')
    prev_hash_bytes = hex_string_to_byte_array(prev_hash)

    while not proof_found:
        nonce = generate_nonce()
        nonce_bytes = nonce.to_bytes(4, byteorder='little')
        nonce_bytes = reverse_byte_order(nonce_bytes)
        combined_bytes = prev_hash_bytes + nonce_bytes + quote_bytes
        hash_result = hashlib.sha256(combined_bytes).digest()
        proof_found = is_proof_valid(hash_result)
        if proof_found:
            print("Nonce (int):", nonce)
            print_byte_array(hash_result)

if __name__ == "__main__":
    quote, prev_hash, difficulty = get_block_data()
    calculate_block(quote, prev_hash, difficulty)