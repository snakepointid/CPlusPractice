#include"application\w2v.h"
namespace SNLP {
	int SW2V::build_vocabulary()
	{	  
		//count word frequence
		std::unordered_map<string, int>tokensCount;
		vector<std::pair<string, int>>vocab;
		for (auto &sentence : sentences_)
		{
			if (sentence->tokens_.empty())continue;
			for (auto &token : sentence->tokens_)
			{
				total_text_words_ += 1;
				tokensCount[token] += 1;
			}
		}
		total_distinct_words_ = tokensCount.size();
		printf("the corpus have total %lu [words] with %lu distinct [tokens]\n", total_text_words_, total_distinct_words_);
		if (total_distinct_words_ < 1)return 0;
		//get a index
		vocab.reserve(total_distinct_words_);
		for (auto&pr : tokensCount)
		{
			vocab.emplace_back( pr.first, pr.second);
		}
		auto comp = [](std::pair<string, int>&v1, std::pair<string, int>&v2) {return v1.second > v2.second; };
		std::sort(vocab.begin(), vocab.end(), comp);

		int index = 0;
		wordlist_.reserve(total_distinct_words_);
		for (auto&pr : vocab)
		{
			if (pr.second > min_count_) {
				tokens2index_[pr.first] = index;
				wordlist_.emplace_back(new Word{ pr.first,index,pr.second }); index++;
			}
			else {
				tokens2index_[pr.first] = index;
				wordlist_.emplace_back(new Word{ pr.first,index,pr.second });  
			}	
		 }
		lastIndex_ = index;
		 //initial the word vec
		initialMatrix(1, wordvec_, lastIndex_ + 1, wordDim_);
		initialMatrix(1, negwordvec_, lastIndex_ + 1, wordDim_);
		//initialMatrix(0, wordvecHold_, lastIndex_ + 1, wordDim_);
		//initialMatrix(0, negwordvecHold_, lastIndex_ + 1, wordDim_);
		//transfer the sentences
		for (auto&sentence : sentences_)
		{
			if (sentence->tokens_.empty())continue;
			sentence->indexs_.reserve(sentence->tokens_.size());
			for (auto&token : sentence->tokens_)
			{
				sentence->indexs_.push_back(tokens2index_[token]);
			}
		}
		//set unigram
		unigram_.resize(total_text_words_);
		const float power = 0.75;
		float sum = 0.0;
		for (auto&pr : vocab) { sum += pow((float)pr.second, power); }
 
		int index = 0;
		float colsum = ::pow((float)vocab[index].second, power) / sum;
		for (int a = 0; a<unigram_.size(); a++)
		{
			if (float(a) / unigram_.size() < colsum) {
				unigram_[a] = index;
			}
			else {
				index++;
				if (index > lastIndex_) { unigram_[a] = lastIndex_; }
				else {
					unigram_[a] = index;
				}		
				colsum+= ::pow((float)vocab[index].second, power) / sum;
			}
		}
		return 1;
	}
	int SW2V::trainWordVec()
	{
		std::cout << "begin to train\n";
		int currIter = 0;
		#pragma omp parallel for
		for (int i = 0; i<sentences_.size();)
		{
			auto &sentence = sentences_[i];
			int trainWords = sentence->indexs_.size();
			if (trainWords >= window_) {
				int ret = _batchTrain(sentence->indexs_);
			}				
		}
		if (trainedSentences > 1000) { TrainedSentences += trainedSentences / 1000; trainedSentences = trainedSentences % 1000; }
		printf("total trained sentences: %luk\n", TrainedSentences);

	}
	int SW2V::getSimilarWords()
	{
 
		for (auto&w : wordvecNorm_)SVC::norml2(w);
		string token;
		vector<std::pair<int, float>>dist;
		dist.reserve(indexs2tokens_.size());
		while (1)
		{
			printf("please enter the word you want to test('#quit' to quit):");
			std::cin >> token;
			if (token == "#quit")break;
			printf("the word you want to test is' %s '\n", token.c_str());
			lowerTokens(token);
			auto it = tokens2indexs_.find(token.c_str());
			if (it == tokens2indexs_.end()) { printf("word is not exist!!!\n"); continue; }
			Fvector &targVec = wordvecNorm_[it->second];

			for (int idx = 0; idx<wordvecNorm_.size(); idx++)
			{
				dist.push_back(std::move(std::make_pair(idx, SVC::dot(targVec, wordvecNorm_[idx]))));
			}
			auto comp = [](std::pair<int, float>&v1, std::pair<int, float>&v2) {return v1.second > v2.second; };
			std::sort(dist.begin(), dist.end(), comp);
			for (int i = 0; i < 11; i++) {
				if (it->second == dist[i].first)continue;
				printf("%lu most similar: %s \n", i, indexs2tokens_[dist[i].first].c_str());
			}
			printf("---------------------------------------------------------------------------\n");
			dist.clear();
			token.clear();
		}

	}

	int SW2V::saveWordVec(string & fn)
	{
		std::ofstream fin;
		fin.open(fn);
		fin << wordvec_.size() << " " << wordvec_[0].size() << "\n";
		for (int idx = 0; idx < indexs2tokens_.size(); idx++)
		{
			fin << indexs2tokens_[idx] << " ";
			for (auto&w : wordvec_[idx]) { fin << w << " "; }
			fin << "\n";
		}
		fin.close();
	}

	int SW2V::loadWordVec(string & fn)
	{
		wordvec_.clear();
		std::ifstream fin;
		string line;
		string token;
		fin.open(fn);
		if (!getline(fin, line)) { std::cout << "there have no data\n"; return -1; }
		int n_words = 0, wordDim = 0;
		std::stringstream(line) >> n_words >> wordDim;
		wordvec_.resize(n_words);
		for (auto&w : wordvec_)w.resize(wordDim);
		int idx = 0;
		float wt;
		while (getline(fin, line))
		{
			std::stringstream ipt(line);
			ipt >> token;

			int widx = 0;
			while (ipt >> wt)
			{
				wordvec_[idx][widx++] = wt;
			}
			tokens2indexs_[token] = idx++;
			indexs2tokens_.push_back(move(token));
		}
		fin.close();
		return 1;
	}

	int SW2V::loadRawText(string & fn)
	{
		std::ifstream fin(fn);
		SentenceP sentence(new Sentence);
		string line;
		string token;
		while (std::getline(fin, line))
		{
			if (line.empty())continue; 
			sentences_.emplace_back(new Sentence{splits(line, ' ')});
		}
		fin.close();
		printf("have loaded %lu sentences\n", sentences_.size());
	}

	int SW2V::_initPara()
	{
		printf("the default parameters' values are:\n------------------------------------\n");
		printf("#1 alpha     : %.4f---control the model learning rate \n", alpha_);
		printf("#2 min_alpha : %.4f---control the model smallest learning rate,because the learning rate decay in Adam \n", min_alpha_);
		printf("#3 window    : %lu---the context window size \n", window_);
		printf("#4 wordDim   : %lu---the word vec dimemsion \n", wordDim_);
		printf("#6 min_count : %lu---determine which word is infrequency word \n", min_count_);
		printf("choose the parameter number(eta. alpha is 1) you want to change(0 to quit):");
		int c = 0;
		std::cin >> c;
		while (c != 0)
		{
			switch (c)
			{
			case 0: break;
			case 1: printf("\nalpha     :"); std::cin >> alpha_; break;
			case 2: printf("\nmin_alpha :"); std::cin >> min_alpha_; break;
			case 4: printf("\nwindow    :"); std::cin >> window_; break;
			case 5: printf("\nwordDim   :"); std::cin >> wordDim_; break;
			case 7: printf("\nmin_count :"); std::cin >> min_count_; break;
			default:printf("no such parameter,please choose again"); break;
			}
			printf("choose the parameter you want to change(0 to quit):");
				std::cin >> c;
		}
	}

	int SW2V::_batchTrain(Ivector & sentence)
	{
		
	}
	 
}