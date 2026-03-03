import secrets
import string
import os

class JennyVault:
    def generate_password(self, length=16):
        alphabet = string.ascii_letters + string.digits + string.punctuation
        password = ''.join(secrets.choice(alphabet) for i in range(length))
        return password

    def save_password(self, filename, password):
        try:
            with open(f"{filename}.txt", "w") as f:
                f.write(f"Password: {password}")
            return True
        except Exception:
            return False
