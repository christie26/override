# Get login input from the user
login = input("Enter login: ")

if len(login) < 6:
    raise ValueError("Login must be at least 6 characters long")

char_value = ord(login[3])  # Get ASCII value of the fourth character
v4 = (char_value ^ 0x1337) + 0x5eeded

# Loop through each character in the string and perform the operations
for char in login:
    char_value = ord(char)  # Get ASCII value of the character
    v4 += (v4 ^ char_value) % 0x539

print(f'Serial: {v4}')
