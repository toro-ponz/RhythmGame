#include "FrameNumberClass.h"

namespace Frame {
	/**
	*  �R���X�g���N�^
	*/
	FrameNumber::FrameNumber() {
		frameNumber = FRAME_NUMBER::CREDIT1_INIT;
	}

	/**
	*  �f�X�g���N�^
	*/
	FrameNumber::~FrameNumber() {

	}

	/**
	*  �t���[���i���o�[���w�肷��֐�.
	*  @param fNum<FRAME_NUMBER> �o�^����t���[���i���o�[
	*/
	void FrameNumber::setFrameNumber(FRAME_NUMBER fNum) {
		frameNumber = fNum;
	}

	/**
	*  ���݂̃t���[���i���o�[��Ԃ��֐�.
	*  @return <FRAME_NUMBER> ���݂̃t���[���i���o�[
	*/
	FrameNumber::FRAME_NUMBER FrameNumber::getFrameNumber() {
		return frameNumber;
	}

	/**
	*  ���݂̃t���[���i���o�[�Ǝw�肳�ꂽ���̂������������肷��֐�.
	*  @param fNum<FRAME_NUMBER> ���肷��t���[���i���o�[
	*  @return <bool> fNum�����݂̃t���[���i���o�[�Ɠ��������true
	*/
	bool FrameNumber::compareFrameNumber(FRAME_NUMBER fNum) {
		return frameNumber == fNum;
	}
}