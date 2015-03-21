#ifndef TICKER_ADDON_H
#define TICKER_ADDON_H

struct TickerAddonInfo {
const char *SiteHost;
const char *SiteFile;
const char *SiteName;
};

extern "C" 
{
	_EXPORT parse(const char *buffer, size_t bufferlen, char **output, size_t *outnum);
	_EXPORT TickerAddonInfo Info = { "www.beoscentral.com", "/powerbosc.txt", "BeOS Central" };
}