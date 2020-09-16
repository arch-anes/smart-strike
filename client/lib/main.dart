import 'package:flutter/material.dart';

import 'striker.dart';

void main() => runApp(SmartStrike());

class SmartStrike extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart Strike',
      home: Scaffold(
        appBar: AppBar(
          title: Text('Smart Strike'),
        ),
        body: Material(
          color: Colors.white,
          child: Center(
            child: Ink(
              decoration: const ShapeDecoration(
                color: Colors.lightBlue,
                shape: CircleBorder(),
              ),
              child: IconButton(
                icon: Icon(Icons.sensor_door),
                color: Colors.white,
                onPressed: () {
                  Striker.strike();
                },
              ),
            ),
          ),
        ),
      ),
    );
  }
}
