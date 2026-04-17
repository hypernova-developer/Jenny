#ifndef JENNY_VAULT_HPP
#define JENNY_VAULT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>

class JennyVault
{
public:
    std::string generatePassword(int length = 16)
    {
        const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{}|;:,.<>?";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, characters.size() - 1);

        std::string password = "";
        for (int i = 0; i < length; ++i)
        {
            password += characters[distribution(generator)];
        }
        return password;
    }

    bool savePassword(std::string filename, std::string password)
    {
        std::ofstream file(filename + ".txt");
        if (file.is_open())
        {
            file << "Password: " << password;
            file.close();
            return true;
        }
        return false;
    }
};

#endif