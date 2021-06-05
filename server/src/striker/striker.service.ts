import { Injectable, HttpService } from '@nestjs/common';

@Injectable()
export class StrikerService {
    deviceAuth = {} as { host: string; user: string; pass: string };

    constructor(private httpService: HttpService) {
        try {
            this.deviceAuth = JSON.parse(process.env.DEVICE_AUTH);
        } catch (e) {
            throw new Error('Undefined device authentication information');
        }
    }

    strike() {
        return this.httpService
            .post(
                `http://${this.deviceAuth.host}`,
                {},
                {
                    auth: {
                        username: this.deviceAuth.user,
                        password: this.deviceAuth.pass,
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
