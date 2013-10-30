<?php

/**
 * @file
 * Alternate /user callback.
 */

/**
 * Page callback to use in place of core's user_page().
 */
function delftdora_user_page() {
  global $user;
  if ($user->uid) {
    menu_set_active_item('user/' . $user->uid);
    return menu_execute_active_handler(NULL, FALSE);
  }
  else {
    return array(
      '#type' => 'markup',
      'login' => array(
        '#prefix' => '<div id="user-login-container"><h2>' . t('Sign in') . '</h2>',
        '#suffix' => '</div>',
        'form' => drupal_get_form('user_login'),
        'forgotten_pass' => array(
          '#prefix' => '<a href="' . url('user/password') . '">',
          '#suffix' => '</a>',
          '#markup' => t('Forgotten password?'),
        ),
      ),
      'register' => array(
        '#prefix' => '<div id="user-register-form-container"><h2>' . t('Create an account') . '</h2>',
        '#suffix' => '</div>',
        '#access' => user_register_access(),
        'form' => drupal_get_form('user_register_form'),
        'privacy_policy' => array(
          '#prefix' => '<a href="' . url('privacy-policy') . '">',
          '#suffix' => '</a>',
          '#markup' => t('Privacy policy'),
        ),
      ),
    );
  }
}
