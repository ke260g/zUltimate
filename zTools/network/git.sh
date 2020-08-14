#!/bin/bash

# 远程有别人push了，导致本地push不成功
git pull  --allow-unrelated-histories

# At the first time, to `Obtain` the whole remote-repo.
git clone "https://your/url/to/git/repo"

# To `Save` your username and passwd after your first push and pull.
git config --global credential.helper store

# A real-day routine is showed as follow
# [1] `Sync` the remote-repo before you change the repo.
git pull
:<<! [2] editing
    Edit your code, debug, ...
!
# [3] `Add` your change to the local-repo.
git add -A
# [4] `Record` your commit for your new changing.
git commit
# [5] `Update` your local-disk change to the remote-repo.
git push

# roll back
# Note: Be careful, the un-pushed changes will be discarded.
# [1] `rollback` a single file
git checkout "<hash code>"  "<path/to/file>"
# [2] `rollback` the whole project
git reset --hard "<hash code>"

# commit editor
git config --global core.editor vim

# save password
git config --global user.email  ""
git config --global user.name   ""
git config --global credential.helper store
git config --global credential.helper 'cache --timeout=3600'
# clear password
git config --global --unset credential.helper

# using git as clond note
git add -A && git commit --allow-empty-message -m "" && git push --force

# init
git init && git add -A && git commit -m "" --allow-empty-message
git remote add origin $url && git push -u origin master