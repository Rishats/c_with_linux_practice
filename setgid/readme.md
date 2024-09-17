### Setting up the `testgroupsetgid` group and permissions for `main1` and `main2`

#### 1. Creating the `testgroupsetgid` group

First, you need to create a new group that will manage access to the files.

```bash
sudo groupadd testgroupsetgid
```

Verify that the group was created successfully:

```bash
getent group testgroupsetgid
```

#### 2. Compiling `main1` and `main2`

##### 2.1 Creating and compiling `main1`

Here is an example of the `main1.c` program, which calls `main2`:

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Executing main2 from main1...\n");
    system("./main2");
    return 0;
}
```

Compile `main1.c` into an executable:

```bash
gcc -o main1 main1.c
```

##### 2.2 Creating and compiling `main2`

The `main2.c` program simply prints a message:

```c
#include <stdio.h>

int main() {
    printf("Hello from main2!\n");
    return 0;
}
```

Compile `main2.c`:

```bash
gcc -o main2 main2.c
```

#### 3. Setting permissions for `main1` and `main2`

##### 3.1 Assigning the `testgroupsetgid` group to the files

Assign the newly created group `testgroupsetgid` to both `main1` and `main2`:

```bash
sudo chown :testgroupsetgid main1 main2
```

Check the owner and group of the files:

```bash
ls -l main1 main2
```

Both files should belong to the `testgroupsetgid` group, for example:

```
-rwxr-sr-x 1 root testgroupsetgid 16K Sep 13 00:36 main1
-rwxrwx--- 1 root testgroupsetgid 16K Sep 13 00:36 main2
```

##### 3.2 Setting permissions for `main2`

Set `main2` to be executable only by the owner and the group:

```bash
sudo chmod 750 main2
```

##### 3.3 Setting the **SetGID** bit for `main1`

To allow `main1` to run with the group privileges of `testgroupsetgid`, set the **SetGID** bit:

```bash
sudo chmod g+s main1
```

Verify the result:

```bash
ls -l main1
```

You should see the letter `s` in the group permissions, indicating that the **SetGID** bit is set:

```
-rwxr-sr-x 1 root testgroupsetgid 16K Sep 13 00:36 main1
```

#### 4. Verifying execution

##### 4.1 Creating the `skeleton` user

To test program execution, create a new user:

```bash
sudo useradd skeleton
```

##### 4.2 Adding the user to the `testgroupsetgid` group

Add the user to the `testgroupsetgid` group so they can execute `main1`:

```bash
sudo usermod -aG testgroupsetgid skeleton
```

##### 4.3 Checking permissions

Log in as the `skeleton` user and attempt to run `main2` directly. The user should not have permission to execute it:

```bash
su - skeleton
./main2
# Output: zsh: permission denied: ./main2
```

However, the user can run `main1`, which will call `main2` using the group privileges of `testgroupsetgid`:

```bash
./main1
# Output: Executing main2 from main1...
# Hello from main2!
```

#### 5. Conclusion

- The `main1` program uses the **SetGID** bit to run with the group privileges of `testgroupsetgid`, allowing it to execute `main2`.
- The `skeleton` user cannot directly execute `main2` due to permission restrictions, but can execute it indirectly via `main1`.

#### 6. Summary of commands

- Create the `testgroupsetgid` group:  
  `sudo groupadd testgroupsetgid`
  
- Compile `main1.c` and `main2.c`:  
  `gcc -o main1 main1.c`  
  `gcc -o main2 main2.c`
  
- Assign the `testgroupsetgid` group to both files:  
  `sudo chown :testgroupsetgid main1 main2`
  
- Set the appropriate permissions:  
  `sudo chmod 750 main2`  
  `sudo chmod g+s main1`

- Add a user for testing:  
  `sudo useradd skeleton`  
  `sudo usermod -aG testgroupsetgid skeleton`
