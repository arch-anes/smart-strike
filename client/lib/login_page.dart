import 'package:flutter/material.dart';
import 'package:flutter_modular/flutter_modular.dart';

import 'login_service.dart';

class LoginPage extends StatelessWidget {
  final loginService = Modular.get<LoginService>();

  final TextEditingController _passwordController = TextEditingController();
  final passwordError = ValueNotifier<String>(null);

  @override
  Widget build(BuildContext context) {
    return Padding(
        padding: EdgeInsets.all(10),
        child: ListView(children: <Widget>[
          Container(
              alignment: Alignment.center,
              padding: EdgeInsets.all(15),
              child: Text(
                'Enter the password',
                style: TextStyle(
                    color: Colors.blue,
                    fontWeight: FontWeight.w500,
                    fontSize: 30),
              )),
          Container(
              padding: EdgeInsets.fromLTRB(10, 10, 10, 0),
              child: ValueListenableBuilder(
                  valueListenable: passwordError,
                  builder: (BuildContext context, String error, Widget child) =>
                      AutofillGroup(
                          child: TextField(
                        autofillHints: const <String>[AutofillHints.password],
                        obscureText: true,
                        controller: _passwordController,
                        decoration: InputDecoration(
                            border: OutlineInputBorder(),
                            labelText: 'Password',
                            errorText: error),
                      )))),
          Container(
              height: 50,
              margin: EdgeInsets.fromLTRB(0, 10, 0, 0),
              padding: EdgeInsets.fromLTRB(10, 0, 10, 0),
              child: RaisedButton(
                textColor: Colors.white,
                color: Colors.blue,
                child: Text('Login'),
                onPressed: () async {
                  passwordError.value = null;
                  final success =
                      await loginService.tryLogin(_passwordController.text);
                  passwordError.value =
                      success ? null : "You have entered the wrong password";
                },
              )),
        ]));
  }
}
