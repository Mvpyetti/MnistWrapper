// This is the main DLL file.

#include "stdafx.h"

#include "MnistWrapper.h"

MnistWrapper::MnistWrapperClass::MnistWrapperClass() {
	NN = new NeuralNetwork();
	dataFile = new MNIST;
	epochCount = new double;

		//generate the mapper for enumerators
	functionMapper = new map<string, func>;
	functionMapper->insert(pair<string, func>("TANH", TANH));
	functionMapper->insert(pair<string, func>("SIGMOID", SIGM));
	functionMapper->insert(pair<string, func>("DOUBLE SIG", DBLSIG));
	functionMapper->insert(pair<string, func>("ReLU", RELU));

}

void MnistWrapper::MnistWrapperClass::SetActFunc(System::String^ inFunc) {	
	//NOTE: We have to use this weird system string so that C# can pass in string values
	char* charPtr = (char*)Marshal::StringToHGlobalAnsi(inFunc).ToPointer();
	string strInFunc(charPtr);
	func enumFunc = (*functionMapper)[strInFunc];
	NN->ChangeActivationFunc(enumFunc);
	Marshal::FreeHGlobal(IntPtr(charPtr));
}

void MnistWrapper::MnistWrapperClass::SetEpochCount(double inEpoch) {
	*epochCount = inEpoch;
}

void MnistWrapper::MnistWrapperClass::SetEta(double inEta) {
	NN->ChangeEta(inEta);
}

void MnistWrapper::MnistWrapperClass::SetNeuronCount(int inNeuronCount) {
	NN->ChangeNeuronCount(inNeuronCount);
}

void MnistWrapper::MnistWrapperClass::TrainNetwork() {
	for (int i = 0; i < *epochCount; i++) {
		for (int j = 0; j < dataFile->GetNumOfImages(); j++) {
			NN->InsertInputs(dataFile->GetImage());
			NN->InsertLabel(dataFile->GetLabel());
			NN->TrainImage();
		}
	}
}

void MnistWrapper::MnistWrapperClass::TestNetwork() {
	for (int i = 0; i < dataFile->GetNumOfImages(); i++) {
		NN->InsertInputs(dataFile->GetImage());
		NN->InsertLabel(dataFile->GetLabel());
		NN->TestImage();
	}
}

bool MnistWrapper::MnistWrapperClass::ReadImages(System::String^ strFile) {
	char* charPtr = (char*)Marshal::StringToHGlobalAnsi(strFile).ToPointer();
	string cstrFile = string(charPtr);
	Marshal::FreeHGlobal(IntPtr(charPtr));
	if (dataFile->ReadInputFile(cstrFile)) {
		NN->ChangeImageCount(dataFile->GetNumOfImages(), *epochCount);
		return true;
	}
	else
		return false;
}

bool MnistWrapper::MnistWrapperClass::ReadLabels(System::String^ strFile) {
	char* charPtr = (char*)Marshal::StringToHGlobalAnsi(strFile).ToPointer();
	string cstrFile = string(charPtr);
	Marshal::FreeHGlobal(IntPtr(charPtr));
	if (dataFile->ReadLabelFile(cstrFile))
		return true;
	else
		return false;
}


bool MnistWrapper::MnistWrapperClass:: FinishedTraining() {
	return NN->IsFinished();
}

bool MnistWrapper::MnistWrapperClass::FinishedReadingImages() {
	return dataFile->finishedReadingInputs;
}

bool MnistWrapper::MnistWrapperClass::FinishedReadingLabels() {
	return dataFile->finishedReadingLabels;
}

int MnistWrapper::MnistWrapperClass::GetTotalImages() {
	return NN->totalImages;
}

int MnistWrapper::MnistWrapperClass::GetImagesRead() {
	return NN->totalImagesRead;
}

int MnistWrapper::MnistWrapperClass::GetEpochIterator() {
	return NN->epochIterator;
}

int MnistWrapper::MnistWrapperClass::GetEpochSize() {
	return NN->epochSize;
}