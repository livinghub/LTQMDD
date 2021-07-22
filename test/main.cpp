/*
 * This file is part of IIC-JKU QFR library which is released under the MIT license.
 * See file README.md or go to http://iic.jku.at/eda/research/quantum/ for more information.
 */

#include "QuantumComputation.hpp"
#include "algorithms/QFT.hpp"
#include "algorithms/Grover.hpp"
#include "algorithms/GoogleRandomCircuitSampling.hpp"

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {

	//读取real文件
	std::string fileName = "./circuits/test.real";
	if(argc==1)
    {
		std::cout<<"没有输入参数,使用默认电路"<<std::endl;

 	}
	else fileName = argv[1];

	qc::QuantumComputation qc(fileName);

	auto dd = make_unique<dd::Package>(); // create an instance of the DD package
	auto functionality = qc.buildFunctionality(dd);
	//qc.printMatrix(dd, functionality, std::cout); //输出矩阵
	dd::export2Dot(functionality, "test-real.dot"); //输出QMDD表示文件
	std::cout << dd->size(functionality) << std::endl; //输出原始dd的大小

	qc::permutationMap varMap = qc.initialLayout; //复制电路的初始变量映射
	qc::permutationMap ltvarMap = varMap;
	qc.printPermutationMap(varMap); //打印初始映射

	// 调用重排算法
	auto reorderdd = dd->dynamicReorder(functionality, varMap, dd::DynamicReorderingStrategy::Sifting);
	//qc.printMatrix(dd, reorderdd, std::cout); //输出矩阵
	dd::export2Dot(reorderdd, "reorderdd.dot"); //输出QMDD表示文件
	std::cout << dd->size(reorderdd) << std::endl; //输出重排后dd的大小
	qc.printPermutationMap(varMap); //输出重排后的变量映射关系

	// 调用重排算法
	auto ltreod = dd->dynamicReorder(functionality, ltvarMap, dd::DynamicReorderingStrategy::linearSift);
	//qc.printMatrix(dd, reorderdd, std::cout); //输出矩阵
	dd::export2Dot(ltreod, "ltreod.dot"); //输出QMDD表示文件
	std::cout << dd->size(ltreod) << std::endl; //输出重排后dd的大小
	qc.printPermutationMap(ltvarMap); //输出重排后的变量映射关系

	//判断重排前后dd是否一致
	//std::cout << "重排电路是否一致： " << dd->equals(functionality, reorderdd) << std::endl;

	return 0;
}
