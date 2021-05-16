#include "UK5B_out.h"
//#include <boost/date_time/posix_time/posix_time.hpp>



UK5B_out::UK5B_out(const std::string& f)
{
//	const std::string f = "UK." + to_iso_string(boost::posix_time::second_clock::local_time()) + ".csv";
	fout.open(f, std::ios::out);
}

UK5B_out::~UK5B_out()  // NOLINT(bugprone-exception-escape)
{
	if(fout.is_open()) fout.close();
}

void UK5B_out::UK5B_header_csv_print(UK5B_river* river)
{
	const std::string t = ";;;;;;;;;;;;;;";
	fout << lmap["vr"] << ";" << river->vr.UK5B_getValue()  << std::endl;
	fout << lmap["qst"] << ";" << river->qst.UK5B_getValue() << std::endl;
	fout << lmap["dog"] << ";" << river->dog.UK5B_getValue() << std::endl;

	fout << lmap["b"] << ";";
	for (auto& i : river->b.UK5B_getValue()) 	fout << i << " ";
	fout << t << lmap["rb"] << ";";
	for (auto& i : river->rb.first.UK5B_getValue())  fout << i << " ";
	fout << std::endl;
	
	fout << lmap["bb"] << ";" << river->bb.UK5B_getValue() << t;
	fout << lmap["rbb"] << ";" << river->rbb.first.UK5B_getValue() << std::endl;
	
	fout << ";" << t;
	fout << lmap["rw"] << ";" << river->rw.first.UK5B_getValue() << std::endl;
	
	fout << lmap["h"] << ";" << river->h.UK5B_getValue() << t;
	fout << lmap["rh"] << ";" << river->rh.first.UK5B_getValue() << std::endl;

	fout << lmap["l"] << ";";
	for (auto& i : river->l.UK5B_getValue()) fout << i << " ";
	fout << t << lmap["rl"] << ";";
	for (auto& i : river->rl.first.UK5B_getValue()) fout << i << " ";
	fout << std::endl;
	
	fout << lmap["ll"] << ";" << river->ll.UK5B_getValue() << t;
	fout << lmap["rll"] << ";" << river->rll.first.UK5B_getValue() << std::endl;

	fout << lmap["hog"] << ";";
	for (auto& i : river->hog.UK5B_getValue())  fout << i << " ";
	fout << t << lmap["rhog"] << ";";
	for (auto& i : river->rhog.first.UK5B_getValue()) fout << i << " ";
	fout << std::endl;
	
	fout << lmap["nn"] << ";" << river->nn.UK5B_getValue()  << t;
	fout << lmap["dx"] << ";" << river->dx.UK5B_getValue() << std::endl;
	
	fout << lmap["xn"] << ";" << river->xn.UK5B_getValue()  << t;
	fout << lmap["dy"] << ";" << river->dy.UK5B_getValue()  << std::endl;
	
	fout << lmap["cct"] << ";" << river->cct.UK5B_getValue() << t;
	fout << lmap["dz"] << ";" << river->dz.UK5B_getValue() << std::endl;
	
	fout << lmap["n"] << ";" << river->n.UK5B_getValue()   << t;
	fout << lmap["pd"] << ";" << river->pd.UK5B_getValue()   << std::endl;
	
	fout << lmap["nog"] << ";" << river->nog.UK5B_getValue() << t;
	fout << lmap["pc"] << ";" << river->pc.UK5B_getValue() << std::endl;
	
	
}
void UK5B_out::UK5B_body_csv_print()
{

	fout << "=== Тестовый вывод body ===" << std::endl;
	
}
