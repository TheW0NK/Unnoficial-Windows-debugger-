def xor_encrypt(data, key):
    return bytes([b ^ key for b in data])

# Read the hash
with open('keyhash.txt', 'rb') as f:
    hash_data = bytes.fromhex(f.read().decode())

# XOR key (simple for now)
xor_key = 0xAA  
encrypted = xor_encrypt(hash_data, xor_key)

with open('encrypted_hash.bin', 'wb') as f:
    f.write(encrypted)

print(f"Encrypted Hash: {encrypted.hex()}")
