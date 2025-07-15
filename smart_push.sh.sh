#!/bin/bash

# 🌱 Smart script to push code to GitHub (handles first time, auto creates repo)

# Colors
GREEN="\033[1;32m"
CYAN="\033[1;36m"
RESET="\033[0m"

echo -e "${CYAN}---------------------------------------------${RESET}"
echo -e "${GREEN}📦 Checking git repository...${RESET}"

if [ ! -d ".git" ]; then
    echo -e "${GREEN}🛠 No git repo found. Initializing...${RESET}"
    git init

    read -p "📛 Enter new GitHub repo name: " repo_name
    read -p "🌐 Enter description (optional): " repo_desc
    read -p "🔒 Make private? (y/n): " is_private

    # Create repo on GitHub using GitHub CLI
    if [ "$is_private" == "y" ] || [ "$is_private" == "Y" ]; then
        gh repo create "$repo_name" --private --description "$repo_desc" --source=. --remote=origin --push
    else
        gh repo create "$repo_name" --public --description "$repo_desc" --source=. --remote=origin --push
    fi

    git branch -M main
else
    echo -e "${GREEN}✅ Git repository exists.${RESET}"
fi

echo -e "${CYAN}---------------------------------------------${RESET}"
echo -e "${GREEN}📦 Adding all files...${RESET}"
git add .

echo -e "${CYAN}---------------------------------------------${RESET}"
read -p "✏️  Enter commit message: " commit_msg

echo -e "${GREEN}✅ Committing...${RESET}"
git commit -m "$commit_msg"

echo -e "${CYAN}---------------------------------------------${RESET}"
echo -e "${GREEN}🚀 Pushing to GitHub...${RESET}"
git push -u origin main

echo -e "${CYAN}---------------------------------------------${RESET}"
echo -e "${GREEN}✅ All done!${RESET}"

