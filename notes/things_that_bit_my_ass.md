# Detecting file changes : [NEW] , [DELETED] , [MODIFIED] , [INTACT]

## Coding detection of File deletion. 
1. Conceptually , I wanted a function that detected file deletion in `fim.cpp`, since subconsciously I was considering my `main.cpp` as an experimental ground and never thought of it as something that was to be shipped along with the project.

2. For deletion the checks work in the opposite direction. We check the contents of the existing manifest against the current file system paths . Therefore , a separate for loop was added to check file deletion.

3. Very simply I wrote an iterative for loop over the existing manifest , post verifying whether it exists....of-course. I then used the loop to compare the value of the key-value pair stored in the ` "files" ` object of my `manifest.json`.

4. Manifest creation was structured such that if the manifest existed , the recursive then the generate_manifest function would return a false. But this didn't stop our program from re-writing the manifest in memory(not generating it as an `.json` file yet!). A fresh manifest was populated by the recursive iterator in memory which would serve as the object which will be used by the iterator running in the deletion detection loop.

[PROBLEM] : The existing `manifest.json` was not overwritten but the in-memory object no longer contained the path of the deleted file

---

  [FIX]: The fix to this was to stop the recursive travel if the manifest already exists , along with calling the generate_manifest() function call (despite whatever it returns ; `true` or `false`).

---

5. Although we stopped the manifest re-write , we didn't explicitly tell our program what to do when the manifest exists , i.e. the `else` clause. It is very important that we load the `manifest.json` that exists into the `nlohmann::json manifest` object corresponding to that instance/run of our FIM.

---

  [FIX]: The fix is to load the **EXISTING** manifest into our `nlohmann::json ` object using an `else` clause. 

---


