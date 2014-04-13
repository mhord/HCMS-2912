#ifndef __hcms_h_
#define __hcms_h_

class hcms
{
	public:
		hcms::hcms(int numChars);
		void hcms::initDisplay(int numChars);
		void hcms::clearDisplay(int numChars);
	private:
		void hcms::printChar(char printee);
		int _numChars;
		gpio *_resetPin;
		gpio *_blankPin;
		gpio *_rsPin;
};
#endif

