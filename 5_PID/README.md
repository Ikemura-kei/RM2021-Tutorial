# PID Module

## Assignment Submission

1. Duplicate the [PID assignment repository](https://github.com/LIANG-SN/PID-Control-Model) to a new private repository following this [guide](#Duplicate-repository-guide).
2. Follow the instructions on the [PID assignment repository](https://github.com/LIANG-SN/PID-Control-Model) to set up the environment and read the introductions.
3. Implement your code and push it to remote(your private repository)
4. Set our official account,(Username is `HKUST-Enterprize-RoboMasterTeam`) as collaborator by following [This tutorial](https://help.github.jp/enterprise/2.11/user/articles/inviting-collaborators-to-a-personal-repository/).
5. You can feel free to push as many times as you want when you make changes.

## Duplicate repository guide

The repository for the assignment is public and Github does not allow the creation of private forks for public repositories, so you need to follow this instruction to duplicate a private one.

The correct way of creating a private frok by duplicating the repo is documented [here](https://docs.github.com/en/free-pro-team@latest/github/creating-cloning-and-archiving-repositories/duplicating-a-repository).

For this assignment the commands are:

1. Create a bare clone of the repository.(This is temporary and will be removed so just do it wherever.)

    ```bash
    git clone --bare git@github.com:LIANG-SN/PID-Control-Model.git
    ```

2. [Create a new private repository on Github](https://help.github.com/articles/creating-a-new-repository/) and name it `PID-Control-Model`.

3. Mirror-push your bare clone to your new `PID-Control-Model` repository.
    > Replace `<your_username>` with your actual Github username in the url below.

    ```bash
    cd PID-Control-Model    # move into your local repository
    git push --mirror git@github.com:<your_username>/PID-Control-Model.git    # mirror push to remote
    ```

4. Remove the temporary local repository you created in step 1.

    ```bash
    # move out and remove the temporary directory
    cd ..
    rm -rf PID-Control-Model
    ```

5. You can now clone your duplicated `PID-Control-Model` repository on your machine

    ```bash
    git clone git@github.com:<your_username>/PID-Control-Model.git
    ```

6. If you want, add the original repo as remote to fetch (potential) future changes.
    Make sure you also disable push on the remote (as you are not allowed to push to it).

    ```bash
    git remote add upstream git@github.com:LIANG-SN/PID-Control-Model.git
    git remote set-url --push upstream DISABLE
    ```

    You can list all your remotes with `git remote -v`. You should see:

    ```bash
    origin  git@github.com:<your_username>/PID-Control-Model.git (fetch)
    origin  git@github.com:<your_username>/PID-Control-Model.git (push)
    upstream    git@github.com:LIANG-SN/PID-Control-Model.git (fetch)
    upstream    DISABLE (push)
    ```

    > When you push, do so on `origin` with `git push origin`.
