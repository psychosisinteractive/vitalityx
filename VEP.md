# Information on the VEP Format

VEP (Vitality Executable Program) is the standard executable format for VitalityX. It has a lightweight header and the VEP is semi-allowed to choose where it loads within memory.

The struct for defining a VEP header is as follows:

```
typedef struct vepheader {
    ///
    /// VEP signature, should be 'VP'
    ///
    char signature[2];
    ///
    /// VEP flags
    /// 
    char flags;
    ///
    /// Where the VEP should be loaded
    ///
    uint32_t origin;
    ///
    /// VEP size
    ///
    uint32_t vepsize;
    ///
    /// VEP mode
    /// 
    uint32_t mode;
    ///
    /// Reserved data
    /// 
    char reserved[255];
} vepheader_t;
```

After the vepheader is the raw binary which will be loaded in. Example:

```
+---------------------------------------------+--------------------+
|VEPHEADER                                    |PROGRAM DATA        |
|signature flags origin vepsize mode reserved |                    |
+---------------------------------------------+--------------------+
```

The current VEP modes are:
1. VEPMODE_MONOLITHIC, interrupts to system are allowed, no VEP or VAM access
2. VEPMODE_VLIB, interrupts to system are allowed, VEP access is allowed except VAM, shows a consent dialog
3. VEPMODE_VAM, interrupts to system are allowed, VEP access and VAM access is allowed, requires permissions