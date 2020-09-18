import { Injectable, HttpService } from '@nestjs/common';

@Injectable()
export class StrikerService {
    device_hostname = process.env.DEVICE_HOST;
    device_username = process.env.DEVICE_USER;
    device_password = process.env.DEVICE_PASS;

    constructor(private httpService: HttpService) {
        if (this.device_hostname === undefined) throw new Error('Undefined device hostname');
        if (this.device_username === undefined) throw new Error('Undefined device username');
        if (this.device_password === undefined) throw new Error('Undefined device password');
    }

    strike() {
        return this.httpService
            .post(
                `http://${this.device_hostname}`,
                {},
                {
                    auth: {
                        username: this.device_username,
                        password: this.device_password,
                    },
                },
            )
            .toPromise()
            .then(() => 'Success')
            .catch(() => {
                throw new Error("Couldn't reach device");
            });
    }
}
