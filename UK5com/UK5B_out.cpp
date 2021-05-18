#include "UK5B_out.h"
//#include <vector>



UK5B_out::UK5B_out(const std::string& f)
{
	fout.open(f, std::ios::out);
}

void UK5B_out::UK5B_header_print(const UK5B_river& river)
{
	const std::string t = ";;;;;;;;;;;;;;";
	fout << lmap["vr"] << ":" << river.vr.UK5B_getValue()  << std::endl;
	fout << lmap["qst"] << ":" << river.qst.UK5B_getValue() << std::endl;
	fout << lmap["dog"] << ":" << river.dog.UK5B_getValue() << std::endl;

	fout << lmap["b"] << ":";
	for (auto& i : river.b.UK5B_getValue()) 	fout << i << " ";
	fout << t << lmap["rb"] << ":";
	for (auto& i : river.rb.first.UK5B_getValue())  fout << i << " ";
	fout << std::endl;
	
	fout << lmap["bb"] << ":" << river.bb.UK5B_getValue() << t;
	fout << lmap["rbb"] << ":" << river.rbb.first.UK5B_getValue() << std::endl;
	
	fout << ":" << t;
	fout << lmap["rw"] << ":" << river.rw.first.UK5B_getValue() << std::endl;
	
	fout << lmap["h"] << ":" << river.h.UK5B_getValue() << t;
	fout << lmap["rh"] << ":" << river.rh.first.UK5B_getValue() << std::endl;

	fout << lmap["l"] << ":";
	for (auto& i : river.l.UK5B_getValue()) fout << i << " ";
	fout << t << lmap["rl"] << ":";
	for (auto& i : river.rl.first.UK5B_getValue()) fout << i << " ";
	fout << std::endl;
	
	fout << lmap["ll"] << ":" << river.ll.UK5B_getValue() << t;
	fout << lmap["rll"] << ":" << river.rll.first.UK5B_getValue() << std::endl;

	fout << lmap["hog"] << ":";
	for (auto& i : river.hog.UK5B_getValue())  fout << i << " ";
	fout << t << lmap["rhog"] << ":";
	for (auto& i : river.rhog.first.UK5B_getValue()) fout << i << " ";
	fout << std::endl;
	
	fout << lmap["nn"] << ":" << river.nn.UK5B_getValue()  << t;
	fout << lmap["dx"] << ":" << river.dx.UK5B_getValue() << std::endl;
	
	fout << lmap["xn"] << ":" << river.xn.UK5B_getValue()  << t;
	fout << lmap["dy"] << ":" << river.dy.UK5B_getValue()  << std::endl;
	
	fout << lmap["cct"] << ":" << river.cct.UK5B_getValue() << t;
	fout << lmap["dz"] << ":" << river.dz.UK5B_getValue() << std::endl;
	
	fout << lmap["n"] << ":" << river.n.UK5B_getValue()   << t;
	fout << lmap["pd"] << ":" << river.pd.UK5B_getValue()   << std::endl;
	
	fout << lmap["nog"] << ":" << river.nog.UK5B_getValue() << t;
	fout << lmap["pc"] << ":" << river.pc.UK5B_getValue() << std::endl;
	
	
}
void UK5B_out::UK5B_body_print(const UK5B_river& river)
{
	std::vector<std::vector<double>> cut = river.cut;
	const double max = river.max;
	const double tt = river.cct.UK5B_getValue();

	const int h = static_cast<int>(cut.size());
	const int w = static_cast<int>(cut.front().size());

	fout << std::endl;

	for(int i = 0; i <  h - 1; ++i)
	{
		for (int j = 0; j < w; j++)
			fout << cut.at(h - i - 1).at(j) << ";";
		fout << std::endl;
	}
	
	fout << std::endl;
	fout << tt / max << ";" << max << std::endl;
	
}
