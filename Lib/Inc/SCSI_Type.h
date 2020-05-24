
enum class eSCSI_Device_Type {
    DIRECT_ACCESS_BLOCK         = 0x00,	// Direct-access block device (e.g., magnetic disk)	SBC Direct Access Commands	SCSI Block Commands (SBC)
    SEQUENTIAL_ACCESS           = 0x01	// Sequential-access device (e.g., magnetic tape)	SSC Sequential Access Commands	SCSI Stream Commands (SSC)
    PRINTER                     = 0x02	// Printer device	SSC Printer Commands	SCSI Stream Commands (SSC)
    PROCESSOR                   = 0x03	// Processor device	SPC Processor Commands	SCSI Primary Commands (SPC)
    WRITE_ONCE                  = 0x04	// Write-once device	SBC Write Once Commands	SCSI Block Commands (SBC)
    CD_ROM                      = 0x05	// CD/DVD-ROM device	MMC CD-ROM Commands	SCSI Multimedia Commands (MMC)
    SCANNER                     = 0x06	// Scanner device	SGC Scanner Commands	SCSI Graphics Commands (SGC)
    OPTICAL_MEMORY              = 0x07	// Optical memory device (e.g., some optical disks)	SBC Optical Media Commands	SCSI Block Commands (SBC)
    MEDIUM_CHANGER              = 0x08	// Medium changer (e.g. jukeboxes)	SMC Medium Changer Commands	SCSI Medium Changer Commands (SMC)
    COMMUNICATION               = 0x09	// Communications device	SSC Communications Commands	SCSI Stream Commands (SSC)

    //Defined by ASC IT8 (Graphic arts pre-press devices)	ASC IT8 Prepress Commands
    //0x0A
    //0x0B	

    STOAGE_ARRAY_CONTROLLER     = 0x0C	// Storage array controller device (e.g., RAID)	SCC Array Controller Commands	SCSI Controller Commands (SCC)
    ENCLOSURE                   = 0x0D	// Enclosure services device	SES Enclosure Services Commands	SCSI Enclosure Services (SES)
    SIMPLIFIED_DIRECT_ACCESS    = 0x0E	// Simplified direct-access device (e.g., magnetic disk)	RBC Reduced Block Commands	Reduced Block Commands (RBC)
    0x0F	// Optical card reader/writer device	OCRW Optical Card Commands	SCSI Specification for Optical Card Reader/Writer (OCRW)
    0x10	// Reserved for bridging expanders
    0x11	// Object-based Storage Device	OSD Object-based Storage Commands	Object-based Storage Commands (OSD)
    0x12	// Automation/Drive Interface
    0x13	// Security manager device
    0x14	// Host managed zoned block device[2]		Zoned Block Commands (ZBC)
    0x15    // RMC Simplified Multimedia Commands	Reduced Multimedia Commands (RMC)

    // Reserved
    0x16–1Dh	

    0x1E	//Well known logical unit
    0x1F	//Unknown or no device type
} 
