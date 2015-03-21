#include <GraphicsCard.h>
// for pci_info and get_nth_pci_info()
#include <PCI.h>
// for load_add_on()
#include <image.h>
// for debuging
#include <stdio.h>
#include <string.h>
#define xprintf(a) printf a

#define PCI_VENDOR_ID_APPLE						0x106b
#define PCI_DEVICE_ID_APPLE_POSSIBLY_VALKYRIE	0x0007

typedef long (*ctrl_grph_card)(ulong message,void *buf);

extern int main(void) {
	int i = 0;
	pci_info pcii;
	long result;
	status_t status;
	image_id iid;
	area_id	 aid;
	area_info ai;
	ctrl_grph_card cgc;
	graphics_card_spec gcs;
	graphics_card_config gcc;
	graphics_card_info gci;
	graphics_card_hook hooks[B_GRAPHICS_HOOK_COUNT];
	
	// load the add-on
	iid = load_add_on("/boot/home/valkyrie");
	if (iid < B_NO_ERROR) {
		xprintf(("Couldn't load valkyrie driver add-on! %s\n", strerror(iid)));
		return -1;
	}
	status = get_image_symbol(iid, "control_onboard_graphics_card__FUlPv", B_SYMBOL_TYPE_TEXT, &cgc);
	if (status < B_NO_ERROR) {
		status = get_image_symbol(iid, "control_graphics_card", B_SYMBOL_TYPE_TEXT, &cgc);
		if (status < B_NO_ERROR) {
			status = get_image_symbol(iid, "__imp__control_graphics_card", B_SYMBOL_TYPE_TEXT, &cgc);
			if (status < B_NO_ERROR) {
				xprintf(("Couldn't get symbol from add-on! %s\n", strerror(status)));
				return -1;
			}
		}
	}
	xprintf(("control_graphics_card is at 0x%08X\n", cgc));
	
	while (get_nth_pci_info(i++, &pcii) == B_NO_ERROR)
	{
		if((pcii.vendor_id == PCI_VENDOR_ID_APPLE) && (pcii.device_id == PCI_DEVICE_ID_APPLE_POSSIBLY_VALKYRIE))
		{
			xprintf(("Found valkyrie card at pci slot %d\n", i-1));
			// configure the card
			gcs.vendor_id = pcii.vendor_id;
			gcs.device_id = pcii.device_id;
			gcs.screen_base = (void *)pcii.u.h0.base_registers[0];
			result = (cgc)(B_OPEN_GRAPHICS_CARD, &gcs);
			xprintf(("result of ctrl_grph_card(B_OPEN_GRAPHICS_CARD): %s\n", strerror(result)));
		}
	}
	
	result = (cgc)(B_GET_GRAPHICS_CARD_HOOKS, &hooks);
	xprintf(("result of ctrl_grph_card(B_GET_GRAPHICS_CARD_HOOKS): %d\n", result));
	if(result == B_OK)
	{
		for(i=0;i<B_GRAPHICS_HOOK_COUNT;i++)
		{
			xprintf(("hook[%d] = 0x%08X\n", i, hooks[i]));
		}
	}
	
	result = (cgc)(B_GET_GRAPHICS_CARD_INFO, &gci);
	xprintf(("result of ctrl_grph_card(B_GET_GRAPHICS_CARD_INFO): %d\n", result));
	if(result == B_OK)
	{
		xprintf(("version = %d; id = %d; width = %d; height = %d\n", gci.version, gci.id, gci.width, gci.height));
		xprintf(("flags = 0x%08X\n", gci.flags));
		//if(gci.flags & B_CRT_CONTROL)
		//	xprintf(("B_CRT_CONTROL\n"));
		//if(gci.flags & B_GAMMA_CONTROL)
		//	xprintf(("B_GAMMA_CONTROL\n"));
		//if(gci.flags & B_FRAME_BUFFER_CONTROL)
		//	xprintf(("B_FRAME_BUFFER_CONTROL\n"));
		//if(gci.flags & B_PARALLEL_BUFFER_ACCESS)
		//	xprintf(("B_PARALLEL_BUFFER_ACCESS\n"));
		//if(gci.flags & B_LAME_ASS_CARD)
		//	xprintf(("B_LAME_ASS_CARD\n"));
	}

}
