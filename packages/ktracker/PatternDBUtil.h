/*
PatternDBUtil.h

PatternDB utilities

Author: Haiwang Yu, yuhw@nmsu.edu
Created: 08-27-2018
*/

#ifndef _PatternDBUtil_H
#define _PatternDBUtil_H

#include "PatternDB.h"

class PatternDBUtil
{
	public:

	static int BuildPatternDB (const std::string & fin, const std::string & fout);

	static PatternDB* LoadPatternDB (const std::string & fin);

	static TrackletKey EncodeTrackletKey (
			PatternDB::STATION,
  		const unsigned int X, const unsigned int Xp,
			const unsigned int U, const unsigned int Up,
			const unsigned int V, const unsigned int Vp);

	static int Verbosity() {
		return PatternDBUtil::verbosity;
	}

	static void Verbosity(int verbosity) {
		PatternDBUtil::verbosity = verbosity;
	}

	protected:

	PatternDBUtil(){}
	virtual ~PatternDBUtil(){}


	private:

	static int verbosity;
};


#endif /*_PatternDBUtil_H*/