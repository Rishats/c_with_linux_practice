Setting up the `testgroupsetgid` group and permissions for `main1` and `main2`

## 1. Creating the `testgroupsetgid` group

First, you need to create a new group that will be used to manage access to the files.

```bash
sudo groupadd testgroupsetgid
```

Verify that the group was created successfully:

```bash
getent group testgroupsetgid
```

## 2. Compiling `main1` and `main2`

### 2.1 Creating and compiling `main1`

Create your C program `main1.c`. Here’s a simple example of how `main1` could look (this code simply calls `main2`):

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Executing main2 from main1...\n");
    system("./main2");
    return 0;
}
```

Compile `main1.c` into the `main1` binary:

```bash
gcc -o main1 main1.c
```

### 2.2 Creating and compiling `main2`

Similarly, create your `main2.c`. Here’s a simple example:

```c
#include <stdio.h>

int main() {
    printf("Hello from main2!\n");
    return 0;
}
```

Compile `main2.c` into the `main2` binary:

```bash
gcc -o main2 main2.c
```

## 3. Setting permissions for `main1` and `main2`

### 3.1 Assigning the `testgroupsetgid` group to the files `main1` and `main2`

Assign the newly created group to both files:

```bash
sudo chown :testgroupsetgid main1 main2
```

Check the owner and group of the files:

```bash
ls -l main1 main2
```

Both files should now belong to the `testgroupsetgid` group, for example:

```
-rwxr-sr-x 1 root testgroupsetgid 12345 Sep 14 12:34 main1
-rwxr-x--- 1 root testgroupsetgid 12345 Sep 14 12:34 main2
```

### 3.2 Setting permissions for `main2`

The `main2` file should only have execution permissions for the owner and the group:

```bash
sudo chmod 750 main2
```

### 3.3 Setting the **SetGID** bit for `main1`

The `main1` file needs to be executed with the group privileges of `testgroupsetgid`. To achieve this, set the **SetGID** bit:

```bash
sudo chmod g+s main1
```

Verify the result:

```bash
ls -l main1
```

You should see the letter `s` in the group permissions, for example:

```
-rwxr-sr-x 1 root testgroupsetgid 12345 Sep 14 12:34 main1
```

This confirms that the **SetGID** bit is set.

## 4. Verification of execution

### 4.1 Creating the `skeleton` user

Add a new user that will be used to test program execution:

```bash
sudo useradd skeleton
```

### 4.2 Adding the user to the group

If you want the user to have direct access to the programs, you can add them to the `testgroupsetgid` group:

```bash
sudo usermod -aG testgroupsetgid skeleton
```

### 4.3 Checking permissions

Now, the user `skeleton` will not be able to directly run `main2` since they don't have the necessary permissions:

```bash
su - skeleton
./main2
# Output: zsh: permission denied: ./main2
```

However, the user can run `main1`, which will, via the **SetGID** bit, execute `main2`:

```bash
./main1
# main1 runs main2 with the group privileges of testgroupsetgid
```

## 5. Conclusion

- The `main1` file uses the **SetGID** bit to acquire the group permissions of `testgroupsetgid` and is able to execute `main2` on behalf of this group.
- Direct execution of the `main2` file is denied for users who are not members of the `testgroupsetgid` group, but `main1` can run `main2` because it is executed with the group’s permissions.

## 6. Summary of Commands

- Create the `testgroupsetgid` group:  
  `sudo groupadd testgroupsetgid`
  
- Compile `main1.c` and `main2.c` into binaries:  
  `gcc -o main1 main1.c`  
  `gcc -o main2 main2.c`
  
- Assign the group `testgroupsetgid` to both files:  
  `sudo chown :testgroupsetgid main1 main2`
  
- Set the appropriate permissions:  
  `sudo chmod 750 main2`  
  `sudo chmod g+s main1`

- Add a user for testing:  
  `sudo useradd skeleton`  
  `sudo usermod -aG testgroupsetgid skeleton`
```
