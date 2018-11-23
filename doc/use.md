## �g����
<a href="https://github.com/inonote/SpringScriptEngine/releases">Releases</a>����ŐV�ł��_�E�����[�h���AC++�̏ꍇ��`SpringScriptEngine.dll`�A`SpringScriptEngine.h`�A`SpringScriptEngine.lib`���AHSP�̏ꍇ��`SpringScriptEngine.dll`�A`SpringScriptEngine.as`���g�������v���O�����̃\�[�X�R�[�h�t�H���_�ɃR�s�[���܂��B
���C�u�����̊֐��̃��t�@���X�́A<a href="_spring_script_engine_8h.html">�����ɂ���܂�</a>�B

## ��{�I�ȃR�[�h  
*C(C++)*
```C
#include "SpringScriptEngine.h"
#pragma comment(lib, "SpringScriptEngine.lib")
HSPRINGSCRIPT hSpringScript = NULL;
int main()
{
	/* SpringScriptEngine�̃C���X�^���X���쐬 */
	hSpringScript = SpringScript_CreateInstance();
	
	/* �����ɖ��ߓo�^�A�X�N���v�g�ǂݍ��݁A���s�̃R�[�h�������܂��B */
	
	/* �C���X�^���X��j�� */
	SpringScript_DestroyInstance(hSpringScript);
	
	return 0;
}
```

*HSP3*
```C
#include "SpringScriptEngine.as"
#include "modclbk3.hsp"

//�X�N���v�g�G���W���̃C���X�^���X���쐬
hSpringScript = SpringScript_CreateInstance()

//�����ɖ��ߓo�^�A�X�N���v�g�ǂݍ��݁A���s�̃R�[�h�������܂��B

//�X�N���v�g�G���W���̃C���X�^���X��j��
SpringScript_DestroyInstance hSpringScript
stop
```

## �T���v���R�[�h
`SpringScriptEngine_vX.X.X/Samples/`��C++��HSP3�̃T���v���R�[�h������܂��B