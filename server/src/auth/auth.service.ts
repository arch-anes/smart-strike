import { Injectable } from '@nestjs/common';
import { JwtService } from '@nestjs/jwt';
import { AuthPayload } from './auth.interfaces';
import { compareSync } from 'bcryptjs';

@Injectable()
export class AuthService {
    encryptedAppPassword = process.env.APP_PASS;

    constructor(private jwtService: JwtService) {
        if (this.encryptedAppPassword === undefined) throw new Error('Undefined encrypted app password');
    }

    login(payload: AuthPayload) {
        if (!compareSync(payload.password, this.encryptedAppPassword)) throw new Error('Incorrect password');

        return this.generateToken();
    }

    generateToken() {
        return {
            token: this.jwtService.sign({}),
        };
    }
}
