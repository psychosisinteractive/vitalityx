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
    /// 255 character array for name
    ///
    char name[255];
    ///
    /// Where the VEP should be loaded
    ///
    uint32_t origin;
    ///
    /// VEP size
    ///
    uint32_t vepsize;
} vepheader_t;
```

After the vepheader is the raw binary which will be loaded in. Example

```
+-----------------------------------+--------------------+
|VEPHEADER                          |PROGRAM DATA        |
|signature flags name origin vepsize|                    |
+-----------------------------------+--------------------+
```