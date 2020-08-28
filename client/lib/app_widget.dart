import 'package:flutter/material.dart';
import 'package:flutter_modular/flutter_modular.dart';

import 'login_page.dart';
import 'login_service.dart';
import 'strike_page.dart';

class AppWidget extends StatelessWidget {
  final loginService = Modular.get<LoginService>();

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text('Smart Strike'),
        ),
        body: ValueListenableBuilder(
            valueListenable: loginService.isLoggedIn,
            builder: (BuildContext context, bool isLoggedIn, Widget child) {
              return isLoggedIn
                  ? StrikePage().build(context)
                  : LoginPage().build(context);
            }),
      ),
    );
  }
}
