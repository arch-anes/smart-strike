import React, { useRef, useState } from 'react';
import './App.css';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faDoorOpen } from '@fortawesome/free-solid-svg-icons';
import { Button } from 'react-bootstrap';
import axios from 'axios';

const idToken = new URLSearchParams(window.location.hash.substring(1)).get('id_token');

const authFQDN = 'smart-strike.auth.us-east-1.amazoncognito.com';
const clientId = '45njbimqh80bs86kmgn4d0kc34';
const redirectUrl = window.location.origin + window.location.pathname;
const authorizationUrl = `https://${authFQDN}/oauth2/authorize?client_id=${clientId}&redirect_uri=${redirectUrl}&response_type=token&state=STATE`;

if (idToken === null) {
  window.location.href = authorizationUrl;
}

window.location.hash = '';

const tryOpenDoor = async () =>
  axios
    .post(
      'https://tgfue4mzd5.execute-api.us-east-1.amazonaws.com/smart-strike-api/strike',
      {},
      {
        headers: {
          Authorization: idToken as string,
        },
      },
    )
    .then(() => true)
    .catch((response) => {
      console.error(response);
      return false;
    });

const App = () => {
  const [active, setActive] = useState(false);
  const timerRef = useRef<NodeJS.Timeout | null>(null);

  const handleClick = () =>
    tryOpenDoor().then((success) => {
      if (!success) return;
      setActive(true);
      timerRef.current && clearTimeout(timerRef.current);
      timerRef.current = setTimeout(() => {
        timerRef.current = null;
        setActive(false);
      }, 5000);
    });

  return (
    <div className="App">
      <header className="App-header">
        <Button variant={active ? 'primary' : 'secondary'} size="lg">
          <FontAwesomeIcon icon={faDoorOpen} onClick={handleClick} />
        </Button>
      </header>
    </div>
  );
};

export default App;
