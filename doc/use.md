## 使い方
<a href="https://github.com/inonote/SpringScriptEngine/releases">Releases</a>から最新版をダウンロードし、C++の場合は`SpringScriptEngine.dll`、`SpringScriptEngine.h`、`SpringScriptEngine.lib`を、HSPの場合は`SpringScriptEngine.dll`、`SpringScriptEngine.as`を使いたいプログラムのソースコードフォルダにコピーします。
ライブラリの関数のリファレスは、<a href="_spring_script_engine_8h.html">ここにあります</a>。

## 基本的なコード  
*C(C++)*
```C
#include "SpringScriptEngine.h"
#pragma comment(lib, "SpringScriptEngine.lib")
HSPRINGSCRIPT hSpringScript = NULL;
int main()
{
	/* SpringScriptEngineのインスタンスを作成 */
	hSpringScript = SpringScript_CreateInstance();
	
	/* ここに命令登録、スクリプト読み込み、実行のコードを書きます。 */
	
	/* インスタンスを破棄 */
	SpringScript_DestroyInstance(hSpringScript);
	
	return 0;
}
```

*HSP3*
```C
#include "SpringScriptEngine.as"
#include "modclbk3.hsp"

//スクリプトエンジンのインスタンスを作成
hSpringScript = SpringScript_CreateInstance()

//ここに命令登録、スクリプト読み込み、実行のコードを書きます。

//スクリプトエンジンのインスタンスを破棄
SpringScript_DestroyInstance hSpringScript
stop
```

## サンプルコード
`SpringScriptEngine_vX.X.X/Samples/`にC++とHSP3のサンプルコードがあります。